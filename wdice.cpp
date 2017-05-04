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
using namespace std;

int main(int argc, char *argv[])
{

  unsigned int iDice = max(min(atoi(argv[1]),35),1); // Amount of dice
  unsigned int iDifficulty = max(min(atoi(argv[2]),10),2);

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
	unsigned int iTimesToRoll = min(atoi(argv[3]),100000000); // Times to roll

	// Randomize random number generator
	srand(time(NULL));

	// Per-roll loop
	while(iTimesToRoll-- > 0)
	{

		int successes = 0;
    unsigned int nobotch = 0;
		unsigned int iDiceLeft = iDice; // Amount of dice

		// Per-die loop
		while(iDiceLeft-- > 0)
		{
			// Get result of the die roll
			unsigned int result = ((int) (10.0 * (rand() / (RAND_MAX + 1.0)))) + 1;

			if(result >= iDifficulty) // If result beats difficulty...
			{
				++successes;
        nobotch=1;

				// if specalization-mode is on, give an additional die
				if(argv[4])
				{
					if(atoi(argv[4]))
						if(result==10)
							++successes;
				}
			}
			else if(result==1) // If result is 1, remove a success
				--successes;
		}

		if(successes>0) // If more than one success...
		{
			iPositiveResults[successes]+=1; // Record amount in success array
			iSumSuccesses+=successes; // Add successes to total sum for stats
			++iSuccessRolls; // Increase successful rolls
			if(successes>iHighestSuccess) // If we have a new high record...
			{
				iHighestRollTimes=1; // Reset amount and set highest success value
				iHighestSuccess=successes;
			}
			else if(successes==iHighestSuccess)
				++iHighestRollTimes; // Else increase amount of highest rolls
		}
		else if((successes<0) && (nobotch==0)) // If we have a botch...
		{
			iNegativeResults[abs(successes)]+=1; // Record amount in botch array
			iSumBotches+=successes; // Add 1's to total sum for stats
			++iBotchRolls; // Increase botch rolls
			if(successes<iLowestBotch) // If we have a new low record..
			{
				iLowestRollTimes=1; // Reset amount and set lowest botch value
				iLowestBotch=successes;
			}
			else if(successes==iLowestBotch)
				++iLowestRollTimes; // Else increase amount of lowest rolls
		}
		else
			++iZeroRolls; // Increase amount of failed (0) rolls
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
