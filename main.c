/*SERBOI Florea-Dan - Grupa 315CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main(int argc, char *argv[])
{
	/*vector de frecventa pentru PID-uri*/
	char PID[32768] = {0};
	/*initializam cozile de asteptare si de terminare*/
	TLCoada WaitingQ = NULL, FinishedQ = NULL;
	WaitingQ = InitC(sizeof(TProces));
	FinishedQ = InitC(sizeof(TProces));
	TLProces running_proces = NULL, new_process = NULL;
	int reusita = 1, no_par, idle = 1, quantum, adresa_disp = 0,
	remained_from_quantum;
	char buf_op[30], par[10][30];
	if (argc != 3)
	{
		fprintf(stderr, "Insuficiente argumente\n");
		return -1;
	}
	FILE *input_file = fopen(argv[1], "r");
	if (input_file == NULL)
	{
		fprintf(stderr, "Fisierul \"%s\" nu a fost gasit\n", argv[1]);
		return -2;
	}
	FILE *output_file = fopen(argv[2], "w");
	if (output_file == NULL)
	{
		fprintf(stderr, "Fisierul \"%s\" nu a fost gasit\n", argv[2]);
		return -3;
	}
	int x = fscanf(input_file, "%d\n", &quantum); x++;
	remained_from_quantum = quantum;
	while (fgets(buf_op, 30, input_file))
	{
		/*se extrag parametri operatiei*/
		extract_parameters(buf_op, par, & no_par);
		if (strcmp(par[0], "add") == 0)
		{
			adresa_disp = adresa_minima_disponibila_de_mem_suf(WaitingQ,
				running_proces, atoi(par[1]));
			new_process = AlocProces(min_PID(PID), atoi(par[1]), atoi(par[2]),
				atoi(par[3]), adresa_disp);
			/*daca am memorie suficienta, efectuez defragmentarea*/
			if (adresa_disp + atoi(par[1]) > MAX_ADDRESS)
				reusita = defragmentare(output_file, WaitingQ, running_proces,
					new_process, PID, &adresa_disp);
			if (reusita == 1)
			{
				new_process->address = adresa_disp;
				if (adresa_disp != 0)
					fprintf(output_file,
			"Process created successfully: PID: %d, Memory starts at %#x.\n",
					        new_process->PID, new_process->address);
				else
					fprintf(output_file, 
			"Process created successfully: PID: %d, Memory starts at 0x0.\n",
					        new_process->PID);
				if (idle)
				{
					running_proces = new_process;
					idle = 0;
				}
				else
					inserare_asteptare(WaitingQ, new_process);
				PID[new_process->PID]++;
			}
			else
				DezalocProces(&new_process);
			reusita = 1;
		}
		if (strcmp(par[0], "get") == 0)
			get(output_file, atoi(par[1]), WaitingQ, FinishedQ,
				running_proces);
		if (strcmp(par[0], "push") == 0)
			push_op(output_file, atoi(par[1]), atoi(par[2]), WaitingQ,
				running_proces);
		if (strcmp(par[0], "pop") == 0)
			pop_op(output_file, atoi(par[1]), WaitingQ, running_proces);
		if (strcmp(par[0], "print") == 0 && strcmp(par[1], "stack") == 0)
			print_stack(output_file, search_process(atoi(par[2]), WaitingQ,
				running_proces), atoi(par[2]));
		if (strcmp(par[0], "print") == 0 && strcmp(par[1], "waiting") == 0)
			print_waiting(output_file, WaitingQ);
		if (strcmp(par[0], "print") == 0 && strcmp(par[1], "finished") == 0)
			print_finished(output_file, FinishedQ);
		if (strcmp(par[0], "run") == 0)
			run(atoi(par[1]), WaitingQ, FinishedQ, &running_proces, PID,
				quantum, &idle, &remained_from_quantum);
		if (strcmp(par[0], "finish") == 0)
			finish(output_file, WaitingQ, running_proces);
	}
	void *q = (void *) WaitingQ, *f = (void *) FinishedQ;
	DezalocProces(&running_proces);
	DistrugeC(&q);
	DistrugeC(&f);
	fclose(input_file);
	fclose(output_file);
	return 0;
}
