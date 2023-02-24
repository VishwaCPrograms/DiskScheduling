#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "DSSimul.h"
#include <limits.h>

int request_list_SSTF[200];
int request_list_CSCAN[200];
int requestLength = 0;

int main(int argc, char *argv[]) {
	if (argc > 1 && strlen(argv[1]) > 4) {
		char* request = strtok(argv[1], ",");
		int i = 0;
		while(request) {
			int num = atoi(request);
			request_list_SSTF[i] = num;
			request_list_CSCAN[i] = num;
			request = strtok(NULL, ",");
			i++;
			requestLength++;
		}
	} else {
		srand(time(NULL));
		int i = 0;
		while (i < 50) {
			int flag = 0;
			int random_request = rand() % 200;
			if (requestLength == 0) {
				request_list_SSTF[i] = random_request;
				request_list_CSCAN[i] = random_request;
				i++;
				requestLength++;
			} else {
				for (int j = 0; j < requestLength; j++) {
					if (random_request == request_list_SSTF[j]) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					request_list_SSTF[i] = random_request;
					request_list_CSCAN[i] = random_request;
					i++;
					requestLength++;
				}
	    		}
        	}
	}
	FCFS();
	SSTF();
	C_SCAN();
	return 0;
}

void FCFS() {
	int headMovement = 0;
	printf("FCFS\n");
	printf("Order sequence for FCFS: ");
	for (int i = 0; i < requestLength; i++) {
		if (i != 0) {
			headMovement += abs(request_list_SSTF[i] - request_list_SSTF[i-1]);
		}
		printf("%d ", request_list_SSTF[i]);
	}
	printf("\nTotal head movement: %d", headMovement);
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void SSTF() {
	printf("SSTF\n");
	int currentPos = request_list_SSTF[0];
	int j = 0;
	int headMovement = 0;
	int sstfArray[requestLength];
	int distanceArray[requestLength];
	int longestDelayedTrack = 0;
	int longestDelayedLength = 0;
	int numberOfTracksDelayed = 0;
	double totalDelay = 0;
	for (int k = 0; k < requestLength; k++) {
		int smallestIndex = 0;
		for (int i = 0; i < requestLength; i++) {
			distanceArray[i] = abs(currentPos - request_list_SSTF[i]);
		}
		int minDist = INT_MAX;
		for (int i = 0; i < requestLength; i++) {
			if (minDist > distanceArray[i]) {
				minDist = distanceArray[i];
				smallestIndex = i;
			}
		}
		sstfArray[j] = request_list_SSTF[smallestIndex];
		currentPos = sstfArray[j];
		j++;
		request_list_SSTF[smallestIndex] = INT_MAX;
	}
    	printf("Order sequence for SSTF: ");
	for (int i = 0; i < requestLength; i++) {
		if (i != 0) {
	       		headMovement += abs(sstfArray[i] - sstfArray[i - 1]);
		}
		printf("%d ", sstfArray[i]);
	}
	printf("\nTotal head movement: %d\n", headMovement);

	for (int i = 0; i < requestLength; i++) {
		if (request_list_CSCAN[i] != sstfArray[i]) {
			int track = request_list_CSCAN[i];
			for (int j = 0; j < requestLength; j++) {
				if (sstfArray[j] == track) {
					if ((j - i) > 0) {
						totalDelay += (j - i);
						numberOfTracksDelayed++;
						if ((j - i) > longestDelayedLength) {
							longestDelayedLength = j - i;
							longestDelayedTrack = sstfArray[j];
						}
					}
					break;
				}
			}
		}
	}
	printf("Longest delayed track and length of its delay: %d, %d\n", longestDelayedTrack, longestDelayedLength);
	printf("Average delay: %.2f\n", totalDelay/numberOfTracksDelayed);
    	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

static void sortArray(int trackArray[], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (trackArray[i] < trackArray[j]) {
				int num = trackArray[i];
				trackArray[i] = trackArray[j];
				trackArray[j] = num;
			}
		}
	}
}

void C_SCAN() {
	printf("C SCAN\n");
	int leftSide[requestLength];
	int leftSideSize = 0;
	int rightSide[requestLength];
	int rightSideSize = 0;
	int longestDelayedTrack = 0;
	int longestDelayedLength = 0;
	int numberOfTracksDelayed = 0;
	double totalDelay = 0;
	int headMovement = 0;
	int currentPos = request_list_CSCAN[0];
	int minRight = INT_MAX;
	int minLeft = INT_MAX;
	for (int i = 0; i < requestLength; i++) {
		if (request_list_CSCAN[i] > currentPos) {
			rightSide[rightSideSize] = request_list_CSCAN[i];
			if (minRight > rightSide[rightSideSize]) {
				minRight = rightSide[rightSideSize];
			}
			rightSideSize++;
		} else if (request_list_CSCAN[i] < currentPos) {
			leftSide[leftSideSize] = request_list_CSCAN[i];
			if (minLeft > leftSide[leftSideSize]) {
				minLeft = leftSide[leftSideSize];
			}
			leftSideSize++;
		}
	}
		
	int cscanArray[requestLength + requestLength];
    	printf("Order sequence for C SCAN: ");
	sortArray(leftSide, leftSideSize);
	sortArray(rightSide, rightSideSize);
	cscanArray[0] = currentPos;
	int i = 1;
	int k = 0;
	int j = 0;
	while (k < leftSideSize) {
		cscanArray[i] = leftSide[k];
		i++;
		k++;
	}
	
	if (rightSideSize > 1) {
		if (leftSideSize == 0 || cscanArray[i-1] != 0) {
			cscanArray[i] = 0;
			i++;
			requestLength++;
		}
		
		cscanArray[i] = 199;
		requestLength++;
		i++;
		while (j < rightSideSize) {
			cscanArray[i] = rightSide[j];
			j++;
			i++;
		}
	}

	for (int i = 0; i < requestLength; i++) {
		if (i != 0) {
	       		headMovement += abs(cscanArray[i] - cscanArray[i - 1]);
		}
		printf("%d ", cscanArray[i]);
	}
	printf("\nTotal head movement: %d\n", headMovement);

	for (int i = 0; i < requestLength; i++) {
		if (request_list_CSCAN[i] != cscanArray[i]) {
			int track = request_list_CSCAN[i];
			for (int j = 0; j < requestLength; j++) {
				if (cscanArray[j] == track) {
					if ((j - i) > 0) {
						totalDelay += (j - i);
						numberOfTracksDelayed++;
						if ((j - i) > longestDelayedLength) {
							longestDelayedLength = j - i;
							longestDelayedTrack = cscanArray[j];
						}
					}
					break;
				}
			}
		}
	}
	printf("Longest delayed track and length of its delay: %d, %d\n", longestDelayedTrack, longestDelayedLength);
	printf("Average delay: %.2f\n", totalDelay/numberOfTracksDelayed);
}
