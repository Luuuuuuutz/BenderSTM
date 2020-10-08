#include "gcode.h"

void openFileG(FATFS* FatFs, FIL Fil)
{
	FRESULT res;

	int result = sd_initialize();

	if (result || 0)
		return;

	res = f_mount(FatFs, "", 0);

	res = f_open(&Fil, "simple.gc", FA_READ | FA_OPEN_EXISTING);

	//if (res)
	//	return;

}

void readGCodeLine(FATFS* FatFs, FIL Fil)
{

    char* data_ptr;
    char* eptr;
    char line[100];
    char G[10] = {0};

    char M[10] = {0};
    char X[10] = {0};
    char Y[10] = {0};
    char Z[10] = {0};
    char A[10] = {0};
    char B[10] = {0};

    double x_d,y_d,z_d,a_d,b_d;

    int line_index = 0;
    int data_ptr_index = 0;

    TCHAR* res;
	UINT btr = sizeof line / sizeof line[0];


    while(!f_eof(&Fil))
    {

        res = f_gets(line, btr, &Fil);

        while(line[line_index] != '\0')
        {
            if(isalpha(line[line_index]))
            {
                switch(line[line_index])
                {
                    case 'G': data_ptr = G;
                    case 'M': data_ptr = M;
                    case 'X': data_ptr = X;
                    case 'Y': data_ptr = Y;
                    case 'Z': data_ptr = Z;
                    case 'A': data_ptr = A;
                    case 'B': data_ptr = B;
                }
                data_ptr_index = 0;
            }
            else if((isalpha(line[line_index]) == 0) || (line[line_index] == '.'))
            {
                   data_ptr[data_ptr_index] = line[line_index];
                   data_ptr_index++;
            }
            line_index++;
        }

        x_d = strtod(X,&eptr);
        y_d = strtod(Y,&eptr);
        z_d = strtod(Z,&eptr);
        a_d = strtod(A,&eptr);
        b_d = strtod(B,&eptr);

        //Determine which G Code function to call

        if(strncmp(line,"G0",2) == 0)
        {
           G0(x_d,y_d,z_d,a_d,b_d,commandedPosition);
        }

        else if((strncmp(line,"G01",3) == 0) || (strncmp(line,"G1",2) == 0))
        {
            G1(x_d,y_d,z_d,a_d,b_d,commandedPosition);
        }

        else if(strncmp(line,"G28",3) == 0)
        {
            G28(homePosition, commandedPosition);
        }

        else if(strncmp(line,"G28.1",5) == 0)
        {
           G281(x_d,y_d,z_d,a_d,b_d,homePosition);
        }

        else if(strncmp(line,"G92",3) == 0)
        {
           G92(x_d,y_d,z_d,a_d,b_d,actualPosition);
        }
        else if(strncmp(line,"G90",3) == 0)
        {
            mode(ABSOLUTE);
        }
        else if(strncmp(line,"G91",3) == 0)
        {
            mode(RELATIVE);
        }

        memset(line,0,100);
        memset(G,0,10);
        memset(M,0,10);

    }

}

//Functions

void G0(double x_d,double y_d,double z_d,double a_d, double b_d, double *commandedPosition)
{
    commandedPosition[0] = x_d;
    commandedPosition[1] = y_d;
    commandedPosition[2] = z_d;
    commandedPosition[3] = a_d;
    commandedPosition[4] = b_d;
}

void G1(double x_d,double y_d,double z_d,double a_d, double b_d, double *commandedPosition)
{
    commandedPosition[0] = x_d;
    commandedPosition[1] = y_d;
    commandedPosition[2] = z_d;
    commandedPosition[3] = a_d;
    commandedPosition[4] = b_d;
}


void G28(double *homePosition, double *commandedPosition)
{
    commandedPosition[0] = homePosition[0];
    commandedPosition[1] = homePosition[1];
    commandedPosition[2] = homePosition[2];
    commandedPosition[3] = homePosition[3];
    commandedPosition[4] = homePosition[4];
}

void G281(double x_d,double y_d,double z_d,double a_d, double b_d, double *homePosition)
{
    homePosition[0] = x_d;
    homePosition[1] = y_d;
    homePosition[2] = z_d;
    homePosition[3] = a_d;
    homePosition[4] = b_d;
}
void mode(int mode)
{
    mode = 1;
}

void G92(double x_d,double y_d,double z_d,double a_d, double b_d, double *actualPosition)
{
    actualPosition[0] = x_d;
    actualPosition[1] = y_d;
    actualPosition[2] = z_d;
    actualPosition[3] = a_d;
    actualPosition[4] = b_d;
}
