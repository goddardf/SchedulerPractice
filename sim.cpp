#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////
// sample events
#define EVENT1 1
#define EVENT2 2
#define EVENT3 3
#define EVENT4 4

float lambda;
float quantumval;
int type;

////////////////////////////////////////////////////////////////     //event structure
struct event {
	float time;
	int numofProcess;
	int mu;
	float burst;
	float arrival;
	float turnaround;
	float wait;
	int completed = 0;
	int flag;
	struct event* next;
};

////////////////////////////////////////////////////////////////
// function definition
void init();
int run_sim();
void generate_report();
//void schedule_event(struct event*);
void process_event1(struct vector<event> &eve);
void process_event2(struct vector<event> &eve);
void process_event3(struct vector<event> &eve);
void process_event4(struct vector<event> &eve);
float urand();
float genexp();

////////////////////////////////////////////////////////////////
//Global variables
struct event* head; // head of event queue
float clock; // simulation clock3
float turnaround;
float throughput;
float Cpu;
float ProcessReady = 0;

////////////////////////////////////////////////////////////////
//void init()
//{
	// initialize all varilables, states, and end conditions
	// schedule first events
	//processCount = atoi(argv[2]);
	//Scheduletype = atoi(argv[1]);
	//lambda = stof(argv[3]);
	//quantum = atoi(argv[4]);//only in rr
//}
////////////////////////////////////////////////////////////////
void generate(vector<event> &E) {
	int i, n;
	n = 10000;
	for (i = 0; i < n; i++) {

		E[i].burst = genexp();
		

		E[i].arrival = genexp();
		
	}

}
////////////////////////////////////////////////////////////////
void order(event* temp, int n) //double check
{
	event t;
	int i, j;
	for (i = 1; i < n; i++)
		for (j = 0; j < n - i; j++)
		{
			if (temp[j].arrival > temp[j + 1].arrival)
			{
				t = temp[j];
				temp[j] = temp[j + 1];
				temp[j + 1] = t;
			}
		}
}
////////////////////////////////////////////////////////////////
void generate_report()
{
	if (type == 1)
	{
		printf("\n FCFS \n Average Process Ready = %0.2f\n Average turn-around = %0.2f.\n", ProcessReady, turnaround);
		printf(" Throughput = %0.5f\n CPU Utilization = %0.2f percent\n", throughput, Cpu);
	}
	else if(type == 2)
	{
		printf("\n SRTF \n Average Process Ready = %0.2f\n Average turn-around = %0.2f.\n", ProcessReady, turnaround);
		printf(" Throughput = %0.5f\n CPU Utilization = %0.2f percent\n", throughput, Cpu);
	}
	else if (type == 3)
	{
		printf("\n HRRN \n Average Process Ready = %0.2f\n Average turn-around = %0.2f.\n", ProcessReady, turnaround);
		printf(" Throughput = %0.5f\n CPU Utilization = %0.2f percent\n", throughput, Cpu);
	}
	else if (type == 4)
	{
		printf("\n RR \n Average Process Ready = %0.2f\n Average turn-around = %0.2f.\n", ProcessReady, turnaround);
		printf(" Throughput = %0.5f\n CPU Utilization = %0.2f percent\n", throughput, Cpu);
	}
}
////////////////////////////////////////////////////////////////
void process_event1(vector<event> &eve) //FCFS
{ 
	event temp[10000];
	float sumofWait = 0.0;
	float sumofTime = 0.0;
	int x = 0;
	int numberofProc = 10000;
	for (int i = 0; i < numberofProc; i++)
	{
		temp[i] = eve[i];
	}
	order(temp, numberofProc);
	
	sumofWait = temp[0].wait = 0;
	temp[0].turnaround = temp[0].burst - temp[0].arrival;
	sumofTime = temp[0].turnaround;

	for (int i = 1; i < numberofProc; i++) //calc wait and total time
	{
		temp[i].wait = (temp[i-1].burst + temp[i-1].arrival + temp[i-1].wait) - temp[i].arrival;
		temp[i].turnaround = (temp[i].wait + temp[i].burst);
		sumofWait = sumofWait + temp[i].wait;
		sumofTime = sumofTime + temp[i].turnaround;
	}	
	turnaround = sumofTime / numberofProc;
	throughput = numberofProc / (sumofTime + 3);
	Cpu = numberofProc * 100 / (numberofProc + 3);
	//ProcessesReady = 
}
////////////////////////////////////////////////////////////////
void process_event2(vector<event> &eve) //SRTF Shortest Remaining Time First
{
	event temp[10000];
	int totalTime = 0;
	int tempTime;
	int currentTime;
	int b[1000];
	int minburstTime;
	float sumofWait = 0.0;
	float sumofTime = 0.0;
	int numberofProc = 10000;


	for (int i = 0; i < numberofProc; i++)
	{
		temp[i] = eve[i];
		totalTime = totalTime + eve[i].burst;
		
	}
	
	
	order(temp, numberofProc);
	for (int i = 0; i < numberofProc; i++)
	{
		b[i] = temp[i].burst;
	}

	int i = 0;
	int j = 0;
	for (currentTime = 0; currentTime < totalTime; currentTime++)
	{
		if (b[i] > 0 && temp[i].arrival <= currentTime)
		{
			b[i]--;
		}

		if (b[i] <= 0 && temp[i].flag != 1)
		{

			temp[i].flag = 1;
			temp[i].wait = (currentTime + 1) - temp[i].burst - temp[i].arrival;
			temp[i].turnaround = (currentTime + 1) - temp[i].arrival;
			sumofWait = sumofWait + temp[i].wait;
			sumofTime = sumofTime + temp[i].turnaround;
		}
		j = i;
		minburstTime = 999;
		for (int x = 0; x < numberofProc; x++)
		{

			if (temp[x].arrival <= (currentTime + 1) && temp[x].flag != 1)
			{

				if (minburstTime != b[x] && minburstTime > b[x])
				{
					minburstTime = b[x];
					i = x;
				}
			}
		}
	}

	turnaround = sumofTime / numberofProc;
	throughput = numberofProc / (sumofTime + 3);
	Cpu = numberofProc * 100 / (numberofProc + 3);
	//ProcessesReady = 
}
////////////////////////////////////////////////////////////////
void process_event3(vector<event> &eve) //HRRN Highest Response Ratio Next
{
	float sumofWait = 0.0;
	float sumofTime = 0.0;
	int sumofBurst = 0;
	int numberofProc = 10000;
	event temp[10000];
	int i, j, t = 0;
	int totalTime;
	for (int i = 0; i < numberofProc; i++)
	{
		temp[i] = eve[i];
	}
	order(temp, numberofProc);

	for (t = temp[0].arrival; t < sumofBurst;)
	{
		float hrr = -9999;
		float tem;
		int loc;
		for (i = 0; i < numberofProc; i++)
		{
			if (temp[i].arrival <= t && temp[i].completed != 1)
			{
				tem = (temp[i].burst + (t - temp[i].arrival)) / temp[i].burst;
				if (hrr < tem)
				{
					hrr = tem;
					loc = i;
				}
			}
		}
		t += temp[loc].burst;
		temp[loc].wait = t - temp[loc].arrival - temp[loc].burst;
		temp[loc].turnaround = t - temp[loc].arrival;
		turnaround += temp[loc].turnaround;
		temp[loc].completed = 1;
		sumofWait += temp[loc].wait;
		sumofTime += temp[loc].turnaround;

	}
	turnaround = sumofTime / numberofProc;
	throughput = numberofProc / (sumofTime + 3);
	Cpu = numberofProc * 100 / (numberofProc + 3);
	//ProcessesReady = 
}
//////////////////////////////////////////////////////////////// //schedules an event in the future
void process_event4(vector<event> &eve)
{
	int x = 0;
	int pflag = 0;
	int numberofProc = 10000;
	int currentTime = 0;

	int sumofWait = 0;
	int sumofTime = 0;

	event temp1[10001], temp2[10001];

	int i;
	int t;
	for (i = 0; i < numberofProc; i++)
		temp1[i] = eve[i];

	order(temp1, numberofProc);

	for (i = 0; i < numberofProc; i++)
		temp2[i] = temp1[i];


	for (i = 0; i < numberofProc; i++)
	{
		t = 0;
		while (t < numberofProc && temp1[i].burst > 0)
		{
			t++;
			currentTime++;
			temp1[i].burst--;
		}
		if (temp1[i].burst <= 0 && temp1[i].flag != 1)
		{
			temp1[i].wait = currentTime - temp2[i].burst - temp1[i].arrival;
			temp1[i].turnaround = currentTime - temp1[i].arrival;
			pflag++;
			temp1[i].flag = 1;
			sumofWait += temp1[i].wait;
			sumofTime += temp1[i].turnaround;
		}
		if (pflag == numberofProc)
			break;
	}
	turnaround = sumofTime / numberofProc;
	throughput = numberofProc / (sumofTime + (9 * quantumval));
	Cpu = numberofProc * 100 / (numberofProc + (3 * quantumval));
	//ProcessesReady = 
}
//void schedule_event(struct event* new)
//{
	// insert event in the event queue in its order of time
//}
////////////////////////////////////////////////////////////////
// returns a random number between 0 and 1
float urand()
{
	return((float)rand() / RAND_MAX);
}
/////////////////////////////////////////////////////////////
// returns a random number that follows an exp distribution
float genexp()
{
	float u, x;
	x = 0;
	while (x == 0)
	{
		u = urand();
		x = (-1 / lambda) * log(u);
	}
	return(x);
}
////////////////////////////////////////////////////////////
int run_sim(vector<event> &E)
{
	//struct event* eve;
	//while (!end_condition)
	//{
		//eve = head;
		//clock = eve->time;
	switch (type)
	{
	case EVENT1:
		process_event1(E);
		break;
	case EVENT2:
		process_event2(E);
		break;
	case EVENT3:
		process_event3(E);
		break;
	case EVENT4:
		process_event4(E);
		break;
	default:
		return 0;
			// error 
	}

		//head = eve->next;
	//free(eve);
		//eve = NULL;
	//}
	return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	// parse arguments
	struct vector<event> E(10000);	
	type = atoi(argv[1]);
	lambda = atoi(argv[2]);
	int averageserviceTime = atoi(argv[3]);
	quantumval = atoi(argv[4]);//only in rr
	generate(E);
	run_sim(E);
	generate_report();
	return 0;
}