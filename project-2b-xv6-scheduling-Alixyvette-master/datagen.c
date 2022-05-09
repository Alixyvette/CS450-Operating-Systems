#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
//#include "proc.h"

/*#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
int spin() __attribute__((optimize("-O0")));
#endif*/
int spin() 
{
    int i = 0;
    int j = 0;
    int k = 0;
    for(i = 0; i < 1000; ++i)
    {
        for(j = 0; j < 200000; ++j)
        {
            k = j % 10;
            k = k + 1;
        }
    }
    return k;
}

int
main(int argc, char *argv[])
{
   //FILE * fp;
   //fp = fopen("Output.txt", "w");
   //int pid_low = getpid();
   //int pid_med = 0;
   //int pid_high = 0;
   int lowtickets = 100, midtickets = 200, hightickets = 300;
   settickets(lowtickets);
  // struct pstat st;

   if(fork() == 0)
   {   
        if (fork() == 0){
		//high
        	settickets(hightickets);
      	 	//pid_med = getpid();
        	//struct pstat st_before, st_after;
		for (int i = 0; i < 600; i++){
			//getpinfo(&st);
   			//print(&st)
			
			printf(1,"%d,%d,%d\n", getpid(), i, getticks());
			
   			spin();
   		}
		//wait();
//		fclose(fp);
   		exit();
	}
	//mid
   	settickets(midtickets);
        //pid_med = getpid();
        //struct pstat st_before, st_after;
	for (int i = 0; i < 600; i++){
		//getpinfo(&st);
   		//print(&st);
		
		printf(1,"%d,%d,%d\n", getpid(), i, getticks());
		
   		spin();
   	}
	//wait();
//        fclose(fp);
   	exit();
     }   
  
//low
     for (int i = 0; i < 600; i++){
		//getpinfo(&st);
		//print(&st);
		
		printf(1,"%d,%d,%d\n", getpid(), i, getticks());
		
   		spin();
     }
   //fclose(fp);
   wait();
   exit();
}

