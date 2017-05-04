/* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar
 *  14 rue de Plaisance, 75014 Paris, France
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 */
#include <string>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <thread>
using namespace std;


// Struct to store the results of the threaded rolls.
struct threadData {
  unsigned int iDice;
  unsigned int iDifficulty;
  unsigned int iSumSuccesses=0; // Sum of total success dice
  int iSumBotches=0; // Sum of total fail dice
  unsigned int iSuccessRolls=0; // Amount of success rolls
  unsigned int iZeroRolls=0; // Amount of failed rolls
  unsigned int iBotchRolls=0; // Amount of botched rolls
  unsigned int iHighestSuccess=0; // Highest sucess
  int iLowestBotch=0; // Lowest botch
  unsigned int iHighestRollTimes=0; // Amount of highest success
  unsigned int iLowestRollTimes=0; // Amount of lowest botch
  unsigned int iPositiveResults[55] = {0}; // Array of successes
  unsigned int iNegativeResults[55] = {0}; // Array of botches
  unsigned int iTimesToRoll; // Times to roll
  unsigned int iSpec;
};


// Global variables - The threads need to have access to this data.
unsigned int iDice; // Amount of dice
unsigned int iDifficulty; // Roll difficulty
unsigned int iTimesToRoll; // Times to roll
unsigned int iSpec; // Specialization roll?
struct threadData tData[8]; // Results of the threaded rolls.


// Thread payload. This is where the rolls happen.
bool runThread(int tNum)
{
  // Store global data locally so we avoid shared access in the loop
  threadData myData;
  myData.iDice=iDice;
  myData.iDifficulty=iDifficulty;
  myData.iTimesToRoll=(unsigned int) floor(iTimesToRoll/8.0);
  myData.iSpec=iSpec;

  // Other thread-local variables
  int successes;
  unsigned int nobotch;
  unsigned int iDiceLeft;
  unsigned int result;
  unsigned int seed = time(NULL)+tNum; // Store seed thread-locally.

  // Per-roll loop
  while(myData.iTimesToRoll-- > 0)
  {

    successes = 0;
    nobotch = 0;
    iDiceLeft = myData.iDice; // Amount of dice

    // Per-die loop
    while(iDiceLeft-- > 0)
    {
      // Get result of the die roll. rand_r over rand for thread local seed.
      result = ((int) (10.0 * (rand_r(&seed) / (RAND_MAX + 1.0)))) + 1;

      if(result >= myData.iDifficulty) // If result beats difficulty...
      {
        ++successes;
        nobotch=1;

        // if specalization-mode is on, give an additional die
        if(myData.iSpec)
        {
          if(result==10)
            ++successes;
        }
      }
      else if(result==1) // If result is 1, remove a success
        --successes;
    }

    if(successes>0) // If more than one success...
    {
      myData.iPositiveResults[successes]+=1; // Record amount in success array
      myData.iSumSuccesses+=successes; // Add successes to total sum for stats
      ++myData.iSuccessRolls; // Increase successful rolls
      if(successes>myData.iHighestSuccess) // If we have a new high record...
      {
        myData.iHighestRollTimes=1; // Reset amount and set highest success value
        myData.iHighestSuccess=successes;
      }
      else if(successes==myData.iHighestSuccess)
        ++myData.iHighestRollTimes; // Else increase amount of highest rolls
    }
    else if((successes<0) && (nobotch==0)) // If we have a botch...
    {
      myData.iNegativeResults[abs(successes)]+=1; // Record amount in botch array
      myData.iSumBotches+=successes; // Add 1's to total sum for stats
      ++myData.iBotchRolls; // Increase botch rolls
      if(successes<myData.iLowestBotch) // If we have a new low record..
      {
        myData.iLowestRollTimes=1; // Reset amount and set lowest botch value
        myData.iLowestBotch=successes;
      }
      else if(successes==myData.iLowestBotch)
        ++myData.iLowestRollTimes; // Else increase amount of lowest rolls
    }
    else
      ++myData.iZeroRolls; // Increase amount of failed (0) rolls
  }
  tData[tNum] = myData;
  return 1;
}

// Main program
int main(int argc, char *argv[])
{
  // Store argument-specific values for thread access.
  iDice = max(min(atoi(argv[1]),35),1); // Amount of dice
  iDifficulty = max(min(atoi(argv[2]),10),2);
  iTimesToRoll = min(atoi(argv[3]),1000000000); // Times to roll
  if(argv[4])
    if(atoi(argv[4]) == 1)
      iSpec=1;

  // Kick off threads...
  thread t1(runThread,0);
  thread t2(runThread,1);
  thread t3(runThread,2);
  thread t4(runThread,3);
  thread t5(runThread,4);
  thread t6(runThread,5);
  thread t7(runThread,6);
  thread t8(runThread,7);

  // ..and wait for them to be done.
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
  t8.join();

  // Variables needed after all rolls are done.
  unsigned int iSumSuccesses=0; // Sum of total success dice
  int iSumBotches=0; // Sum of total fail dice
  unsigned int iSuccessRolls=0; // Amount of success rolls
  unsigned int iZeroRolls=0; // Amount of failed rolls
  unsigned int iBotchRolls=0; // Amount of botched rolls
  unsigned int iHighestSuccess=0; // Highest sucess
  int iLowestBotch=0; // Lowest botch
  unsigned int iHighestRollTimes=0; // Amount of highest success
  unsigned int iLowestRollTimes=0; // Amount of lowest botch
  unsigned int iPositiveResults[55] = {0}; // Array of successes
  unsigned int iNegativeResults[55] = {0}; // Array of botches

  // Grab all the results of the threads and merge them properly for display.
  int x;
  int y;
  for(x=0;x<8;x++)
  {
     iSuccessRolls+=tData[x].iSuccessRolls;
     iZeroRolls+=tData[x].iZeroRolls;
     iBotchRolls+=tData[x].iBotchRolls;
     iSumSuccesses+=tData[x].iSumSuccesses;
     iSumBotches+=tData[x].iSumBotches;

     if(tData[x].iHighestSuccess >= iHighestSuccess)
     {
       if((tData[x].iHighestSuccess > iHighestSuccess))
       {
         iHighestSuccess=tData[x].iHighestSuccess;
         iHighestRollTimes=tData[x].iHighestRollTimes;
       }
       else
       {
         iHighestRollTimes+=tData[x].iHighestRollTimes;
       }
     }

     if(tData[x].iLowestBotch <= iLowestBotch)
     {
       if((tData[x].iLowestBotch < iLowestBotch))
       {
         iLowestBotch=tData[x].iLowestBotch;
         iLowestRollTimes=tData[x].iLowestRollTimes;
       }
       else
       {
         iLowestRollTimes+=tData[x].iLowestRollTimes;
       }
     }
     for(y=0;y<55;y++)
     {
       iPositiveResults[y]+=tData[x].iPositiveResults[y];
       iNegativeResults[y]+=tData[x].iNegativeResults[y];
     }
  }

	// We're done. Let's output stuff. Amount of successes/fails/botches
  cout << "\nWon: \033[1;32m" << iSuccessRolls << "\033[0m  Fail: " << iZeroRolls << "  Botch: \033[1;31m" << iBotchRolls << "\033[0m" << endl;

	// Calculate percentages of successes/fails/botches
  cout << "Chance: Success -> ";
  if(!iSuccessRolls)
    cout << "\033[1;31m0\033[0m";
  else
    cout << "\033[1;32m" << setprecision(3) << (float(iSuccessRolls) / float(atoi(argv[3]))) * 100.0;
  cout << "%\033[0m  Fail: -> ";
  if(!iZeroRolls)
    cout << "\033[1;32m0";
  else
    cout << setprecision(3) << (float(iZeroRolls) / float(atoi(argv[3]))) * 100.0;
  cout <<  "%\033[0m  Botch: -> ";
  if(!iBotchRolls)
    cout << "\033[1;32m0%\033[0m" << endl;
  else
    cout << "\033[1;31m" << setprecision(3) << (float(iBotchRolls) / float(atoi(argv[3]))) * 100.0 << "%\033[0m" << endl;

	// Output highest success and lowest botch, along with amounts
  cout << "Most successes: \033[1;32m" << iHighestSuccess << "\033[0m (" << iHighestRollTimes << " times)  " << "Least successes: \033[1;31m" << iLowestBotch << "\033[0m (" << iLowestRollTimes << " times)" << endl;

	// Output statistics about all rolled successes and botches
  unsigned int i;
  cout << "Successes:  ";
  for(i=0;i<55;i++)
  {
    if(iPositiveResults[i] > 0)
      cout << setprecision(3) << "\033[1;32m" << i << "\033[0m: " << iPositiveResults[i] << "(\033[1;32m" << (float(iPositiveResults[i]) / float(atoi(argv[3]))) * 100.0 << "%\033[0m)" << "  ";
  }
  cout << endl;
  cout << "Botches:  ";
  for(i=0;i<55;i++)
  {
    if(iNegativeResults[i] > 0)
      cout << setprecision(3) << "\033[1;31m-" << i << "\033[0m: " << iNegativeResults[i] << "(\033[1;31m" << (float(iNegativeResults[i]) / float(atoi(argv[3]))) * 100.0 << "%\033[0m)" << "  ";
  }
  cout << endl;

	// Output average success
  cout << "Average success: ";
  if(!iSuccessRolls)
    cout << "-" << endl;
  else
    cout << "\033[1;32m" << (float(iSumSuccesses) / float(iSuccessRolls)) << "\033[0m" << endl;

	// Output average botch
  cout << "Average botch: ";
  if(!iBotchRolls)
    cout << "-\n" << endl;
  else
    cout << "\033[1;31m" << (float(iSumBotches) / float(iBotchRolls)) << "\033[0m\n" << endl;

	return 0;
}
