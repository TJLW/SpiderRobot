#include "terasic_os.h"
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "CSpider.h"
#include "CSpiderLeg.h"
#include "CMotor.h"
#include "BtSppCommand.h"
#include "mmap.h"
#include "QueueCommand.h"
#include "PIO_LED.h"
#include "PIO_BUTTON.h"
#include <time.h>
#include <iostream>
using namespace std;


bool stringContains(string str, string subStr)
{
  if(str.find(subStr) != string::npos)
    return true;
  else
    return false;
}

bool validLegID(int legID)
{
  if(legID < 0 || legID > 5)
  {
    printf("ERROR - Invalid leg id: %i\r\n", legID);
    return false;
  }
  else
    return true;
}

int main(int argc, char *argv[])
{
  CSpider Spider;
  CQueueCommand QueueCommand;
  int Command, Param;
  bool bSleep = false;
  CPIO_LED LED_PIO;
  CPIO_BUTTON BUTTON_PIO;
  pthread_t id0;
  int ret0;
  uint32_t LastActionTime;
  const uint32_t MaxIdleTime = 10*60*OS_TicksPerSecond(); // spider go to sleep when exceed this time

	printf("Spider Init & Standup\r\n");

	if (!Spider.Init()){
		printf("Spider Init failed\r\n");
	}
  else{
		if (!Spider.Standup())
			printf("Spider Standup failed\r\n");
	}

	Spider.SetSpeed(50);

  printf("\r\n");
	printf("===== Spider Controller =====\r\n");
	printf("Manual Spider Control\r\n");

  printf("Commands:\r\n");
  printf("\r\n");

	int leg = 0;
  string command = "";

	printf("SpiderController# ");
	cin >> command;
  printf("\r\n");

	while(command != "exit")
	{

    // Spider commands
    if(stringContains(command, "spider"))
    {
      // Get spider action
      cin >> command;

      // Reset - sets the legs to base position
      if(stringContains(command, "reset"))
      {
        printf("\tResetting legs...");
        Spider.SetLegsBase();
        printf("DONE\r\n");
      }

      // Extend - extends knees and ankles
      else if(stringContains(command, "extend"))
      {
        printf("\tExtending legs...");
        Spider.Extend();
        printf("DONE\r\n");
      }

      // Fold - Compactly folds legs for easy storage
      else if(stringContains(command, "fold"))
      {
        printf("\tFolding legs...");
        Spider.Fold();
        printf("DONE\r\n");
      }

      // Grab - Bring together fingertips
      else if(stringContains(command, "grab"))
      {
        printf("\tGrabbing with legs...");
        Spider.Grab();
        printf("DONE\r\n");
      }

      else
      {
        printf("ERROR - Invalid spider command: %s\r\n", command.c_str());
      }
    }


    // Hip commands
    if(stringContains(command, "hip"))
    {
      // Expecting leg ID
      cin >> command;
      leg = atoi(command.c_str());

      if(validLegID(leg))
      {

        // Get hip action
        cin >> command;

        // Go to base position
        if(stringContains(command, "relax"))
        {
          printf("\tRelaxing hip %i...", leg);
          Spider.RelaxHip(leg);
          printf("DONE\r\n");
        }

        else
        {
          printf("ERROR - Invalid hip command: %s\r\n", command.c_str());
        }

      }
    }

    // Get the next command
    if(command != "exit")
      printf("SpiderController# ");
    cin >> command;
    if(command != "exit")
       printf("\r\n");
	}

	return 0;
}
