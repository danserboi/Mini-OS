/*SERBOI Florea-Dan - Grupa 315CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*alocam o celula care contine informatia si adresa urmatoarei*/
TLG AlocCelula(void *ax, size_t d)
{
	TLG aux = NULL;
	aux = (TLG) calloc(1, sizeof(TCelulaG));
	if (!aux)
		return NULL;
	aux->urm = NULL;
	aux->info = malloc(d);
	if (!aux->info)
	{
		free(aux);
		return NULL;
	}
	memcpy(aux->info, ax, d);
	return aux;
}

/*initializam o stiva*/
void *InitS(size_t d)
{
	TLStiva s = NULL;
	s = (TLStiva) calloc(1, sizeof(TStiva));
	if (!s)
		return NULL;
	s->dime = d;
	s->vf = NULL;
	return (void *)s;
}

/*introducem un element pe o stiva*/
int Push(void *s, void *ae)
{
	TLG aux = NULL;
	aux = AlocCelula(ae, ((TLStiva)s)->dime);
	if (aux)
	{
		aux->urm = ((TLStiva)s)->vf;
		((TLStiva)s)->vf = aux;
		return 1;
	}
	else
		return 0;
}

/*extragem element de pe stiva*/
int Pop(void *s, void *ae)
{
	memcpy(ae, ((TLStiva)s)->vf->info, ((TLStiva)s)->dime);
	TLG aux = ((TLStiva)s)->vf;
	((TLStiva)s)->vf = aux->urm;
	aux->urm = NULL;
	free(aux->info);
	free(aux);
	return 1;
}

/*distrugem o stiva*/
void DistrugeS(void **aa)
{
	AStiva aas = (AStiva)aa;
	TLG aux = NULL;
	if (*aas)
	{
		while ((*aas)->vf != NULL)
		{
			aux = (*aas)->vf;
			if (aux)
			{
				/*deconecteaza celula din lista*/
				(*aas)->vf = aux->urm;
				/*eliberam spatiul ocupat de element*/
				free(aux->info);
				 /*eliberam spatiul ocupat de celula */
				free(aux);
			}
		}
		free(*aas);
		*aas = NULL;
	}
}

/*afisam o stiva de la baza spre varf*/
void Afisare_stiva(FILE *output_file, TLStiva s)
{
	int x;
	TLStiva sv = s, saux = InitS(sizeof(int));
	if (!saux)
		fprintf(output_file, "Eroare Alocare");
	/*introducem elementele intr-o stiva auxiliara*/
	while (sv->vf)
	{
		Pop(s, &x);
		Push(saux, &x);
	}
	/*acum extrag elementul din stiva auxiliara
	si il afisez in ordinea dorita*/
	while (saux->vf)
	{
		Pop(saux, &x);
		fprintf(output_file, " %d", x);
		Push(s, &x);
	}
	fprintf(output_file, ".\n");
	void *svaux = (void *)saux;
	DistrugeS(&svaux);
}

/*calculam numarul de elemente pe care o stiva il contine*/
int numar_elemente_stiva(TLStiva s)
{
	int nr = 0;
	int x;
	TLStiva sv = s, saux = InitS(sizeof(int));
	if (!saux)
		return -1;
	while (sv->vf)
	{
		Pop(s, &x);
		nr++;
		Push(saux, &x);
	}
	while (saux->vf)
	{
		Pop(saux, &x);
		Push(s, &x);
	}
	void *svaux = (void *)saux;
	DistrugeS(&svaux);
	return nr;
}

/*alocam un nou proces*/
TLProces AlocProces(int PID, int mem_size_in_bytes, int exec_time_in_ms,
                    int priority, int address)
{
	TLProces aux;
	aux = (TLProces) calloc(1, sizeof(TProces));
	if (!aux)
		return NULL;
	aux->urm = NULL;
	aux->stiva_proces = InitS(sizeof(int));
	if (!aux->stiva_proces)
	{
		free(aux);
		return NULL;
	}
	aux->PID = PID;
	aux->mem_size_in_bytes = mem_size_in_bytes;
	aux->exec_time_in_ms = exec_time_in_ms;
	aux->executed_time = 0;
	aux->priority = priority;
	aux->address = address;
	return aux;
}

/*dezalocam un proces*/
void DezalocProces(AProces proces)
{
	if (*proces)
	{
		void *v = (*proces)->stiva_proces;
		DistrugeS(&v);
		free(*proces);
		*proces = NULL;
	}
}

/*initializam o coada*/
void *InitC(size_t d)
{
	TLCoada c;
	c = (TLCoada) calloc(1, sizeof(TCoada));
	if (!c)
		return NULL;
	c->dime = d;
	c->ic = NULL;
	c->sc = NULL;
	return (void *)c;
}

/*introducem un element in coada*/
int IntrQ(void *c, void *ae)
{
	TLProces sfcoada = NULL, aux = (TLProces) ae;
	if (aux)
	{
		if (((TLCoada)c)->ic == NULL)
		{
			((TLCoada)c)->ic = aux;
			((TLCoada)c)->sc = aux;
		}
		else
		{
			sfcoada = ((TLCoada)c)->sc;
			sfcoada->urm = aux;
			((TLCoada)c)->sc = aux;
		}
		return 1;
	}
	else
		return 0;
}

/*extragem un element din coada*/
int ExtrQ(void *c, void **ae)
{
	TLProces aux = ((TLCoada)c)->ic;
	if (((TLCoada)c)->ic != NULL)
	{
		*ae = (void *)aux;
		/*tratez separat cazul cu un singur element in coada*/
		if (((TLCoada)c)->ic == ((TLCoada)c)->sc && ((TLCoada)c)->ic != NULL)
		{
			((TLCoada)c)->ic = NULL;
			((TLCoada)c)->sc = NULL;
		}
		else
		{
			((TLCoada)c)->ic = aux->urm;
			aux->urm = NULL;
		}
		return 1;
	}
	else
		return 0;
}

/*copiem primul element din coada*/
int PrimQ(void *c, void *ae)
{
	void *v = (void *)((TLCoada)c)->ic;
	if (((TLCoada)c)->ic != NULL)
	{
		memcpy(ae, v, ((TLCoada)c)->dime);
		return 1;
	}
	return 0;
}

/*distrugem o coada*/
void DistrugeC(void **aa)
{
	TCoada ** aac = (TCoada **)aa;
	TLProces aux = NULL;
	while ((*aac)->ic != NULL)
	{
		aux = (*aac)->ic; /* adresa celulei eliminate */
		(*aac)->ic = aux->urm;   /* deconecteaza celula din lista */
		DezalocProces(&aux);
	}
	free(*aac);
	*aac = NULL;
}

/*distrugem o copie a unei cozi*/
void DistrugeCopie(void **aa)
{
	ACoada aac = (ACoada)aa;
	TLProces aux = NULL;
	while ((*aac)->ic != NULL)
	{
		aux = (*aac)->ic; /* adresa celulei eliminate */
		(*aac)->ic = aux->urm;   /* deconecteaza celula din lista */
		free(aux);
	}
	free(*aac);
	*aac = NULL;
}

/*cream copia unei cozi*/
int copy_coada(void *c, void *new)
{
	void *v = NULL;
	TLCoada x = (TLCoada) c;
	TLCoada caux = InitC(sizeof(TProces));
	TLProces aux_p = NULL;
	if (!caux)
		return -1;
	while (x->ic)
	{
		aux_p = NULL;
		aux_p = calloc(1, sizeof(TProces));
		if (!aux_p)
			return 0;
		PrimQ(x, aux_p);
		IntrQ(new, aux_p);
		ExtrQ(x, &v);
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(x, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
	return 1;
}

/*calculam numarul de elemente din coada*/
int numar_elemente_coada(TCoada *c)
{
	int nr = 0;
	void *v;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		return -1;
	while (c->ic)
	{
		ExtrQ(c, &v);
		nr++;
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
	return nr;
}

/*se extrag parametri unei operatii*/
void extract_parameters(char operation[30], char parameters[10][30], int *n)
{
	char parameter[30];
	int k;
	unsigned int i;
	k = -1, *n = -1;
	for (i = 0; i <= 9; i++)
		strcpy(parameters[i], "");
	for (i = 0; i <= strlen(operation); i++)
		if (operation[i] != ' ' && operation[i] != '\0'
		        && operation[i] != '\n')
		{
			k++;
			parameter[k] = operation[i];
		}
		else
		{
			parameter[k + 1] = '\0';
			if (strlen(parameter) > 0)
			{
				(*n)++;
				strcpy(parameters[*n], parameter);
			}
			k = -1;
			strcpy(parameter, "");
		}
	(*n)++;
}

/*afisam o stiva, asa cum este cerut de problema*/
void print_stack(FILE *output_file, TLProces process, int PID)
{
	if (process)
	{
		if (numar_elemente_stiva(process->stiva_proces) == 0)
			fprintf(output_file, "Empty stack PID %d.\n", process->PID);
		else
		{
			fprintf(output_file, "Stack of PID %d:", process->PID);
			Afisare_stiva(output_file, process->stiva_proces);
		}
	}
	else
		fprintf(output_file, "PID %d not found.\n", PID);
}

/*afisam o coada in asteptare*/
void print_waiting(FILE *output_file, TCoada *c)
{
	void *v;
	TLProces x;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(output_file, "Eroare Alocare");
	fprintf(output_file, "Waiting queue:\n");
	fprintf(output_file, "[");
	while (c->ic)
	{
		ExtrQ(c, &v);
		x = (TLProces) v;
		/*daca nu e ultimul element*/
		if (c->ic)
			fprintf(output_file, "(%d: priority = %d, remaining_time = %d),\n",
			        x->PID, x->priority, x->exec_time_in_ms);
		/*daca e ultimul nu punem virgula la final*/
		else
			fprintf(output_file, "(%d: priority = %d, remaining_time = %d)",
			        x->PID, x->priority, x->exec_time_in_ms);
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	fprintf(output_file, "]\n");
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
}

/*afisam o coada terminata*/
void print_finished(FILE *output_file, TCoada *c)
{
	void *v;
	TLProces x;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(output_file, "Eroare Alocare");
	fprintf(output_file, "Finished queue:\n");
	fprintf(output_file, "[");
	while (c->ic)
	{
		ExtrQ(c, &v);
		x = (TLProces) v;
		/*daca nu e ultimul element*/
		if (c->ic)
			fprintf(output_file, "(%d: priority = %d, executed_time = %d),\n",
			        x->PID, x->priority, x->executed_time);
		/*daca e ultimul nu punem virgula la final*/
		else
			fprintf(output_file, "(%d: priority = %d, executed_time = %d)",
			        x->PID, x->priority, x->executed_time);
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	fprintf(output_file, "]\n");
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
}

/*calculam PID-ul minim, folosind un vector de frecventa*/
int min_PID(char *PID)
{
	int i;
	for (i = 1; i <= 32767; i++)
		if (PID[i] == 0)
			break;
	return i;
}

/*inseram un proces intr-o coada*/
void inserare_asteptare(TLCoada c, TLProces process)
{
	int introdus_deja = 0;
	void *v;
	TLProces x;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");
	if (numar_elemente_coada(c) != 0)
	{
		while (c->ic)
		{
			ExtrQ(c, &v);
			x = (TLProces) v;
			/*daca am gasit locul in coada, introducem in coada auxiliara
			inaintea elementului curent*/
			if ((x->priority < process->priority ||
				(x->priority == process->priority
					&& x->exec_time_in_ms > process->exec_time_in_ms) ||
			    (x->priority == process->priority
			    	&& x->exec_time_in_ms == process->exec_time_in_ms
			         && x->PID > process->PID )) && introdus_deja == 0)
			{
				IntrQ(caux, process);
				introdus_deja = 1;
			}
			IntrQ(caux, v);
			/*am ajuns la final si nu am introdus elementul*/
			if (c->ic == NULL && introdus_deja == 0)
				IntrQ(caux, process);
		}
		while (caux->ic)
		{
			ExtrQ(caux, &v);
			IntrQ(c, v);
		}
	}
	/*caz separat pentru coada vida*/
	else
	{
		v = (void *) process;
		IntrQ(c, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
}

/*cautam printre toate procesele si afisam procesul dupa cum este cerut*/
void get(FILE *output_file, int PID, TLCoada WaitingQ, TLCoada FinishedQ,
	TLProces running_proces)
{
	int gasit_proces = 0;
	TLProces x;
	void *v;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");

	if (running_proces->PID == PID)
	{
		gasit_proces = 1;
		fprintf(output_file, "Process %d is running (remaining_time: %d).\n",
			PID, running_proces->exec_time_in_ms);
	}

	while (WaitingQ->ic)
	{
		ExtrQ(WaitingQ, &v);
		IntrQ(caux, v);
		x = (TLProces)v;
		if (x->PID == PID && gasit_proces != 1)
		{
			gasit_proces = 1;
			fprintf(output_file, 
				"Process %d is waiting (remaining_time: %d).\n",
				PID, x->exec_time_in_ms);
		}
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(WaitingQ, v);
	}

	while (FinishedQ->ic)
	{
		ExtrQ(FinishedQ, &v);
		IntrQ(caux, v);
		x = (TLProces)v;
		if (x->PID == PID)
		{
			gasit_proces = 1;
			fprintf(output_file, "Process %d is finished.\n", PID);
		}
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(FinishedQ, v);
	}

	if (gasit_proces == 0)
		fprintf(output_file, "Process %d not found.\n", PID);

	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
}

/*cauta un proces dupa PID si returneaza adresa lui*/
TLProces search_process(int PID, TLCoada WaitingQ, TLProces running_proces)
{
	TLProces proces_cautat = NULL;
	int gasit_proces = 0;
	TLProces x;
	void *v;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");

	if (running_proces->PID == PID)
	{
		gasit_proces = 1;
		proces_cautat = running_proces;
	}

	while (WaitingQ->ic)
	{
		ExtrQ(WaitingQ, &v);
		IntrQ(caux, v);
		x = (TLProces)v;
		if (x->PID == PID && gasit_proces != 1)
		{
			gasit_proces = 1;
			proces_cautat = x;
		}
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(WaitingQ, v);
	}

	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);

	if (gasit_proces == 0)
	{
		return NULL;
	}
	return proces_cautat;
}

/*extrage un element de pe stiva procesului*/
void push_op(FILE *output_file, int PID, int data, TLCoada WaitingQ,
	TLProces running_proces)
{
	TLProces process;
	process = search_process(PID, WaitingQ, running_proces);
	if (process == NULL)
	{
		fprintf(output_file, "PID %d not found.\n", PID);
	}
	else
	{
		if (numar_elemente_stiva(process->stiva_proces)
			< process->mem_size_in_bytes / 4)
		{
			Push(process->stiva_proces, &data);
		}
		else
			fprintf(output_file, "Stack overflow PID %d.\n", PID);
	}
}

/*introduce un element pe stiva procesului*/
void pop_op(FILE *output_file, int PID, TLCoada WaitingQ,
	TLProces running_proces)
{
	int data;
	TLProces process;
	process = search_process(PID, WaitingQ, running_proces);
	if (process == NULL)
	{
		fprintf(output_file, "PID %d not found.\n", PID);
	}
	else
	{
		if (numar_elemente_stiva(process->stiva_proces) > 0)
		{
			Pop(process->stiva_proces, &data);
		}
		else
			fprintf(output_file, "Empty stack PID %d.\n", PID);
	}
}

/*calculam memoria ocupata de toate procesele*/
int memorie_ocupata(TLCoada c, TLProces running_proces, TLProces new_process)
{
	int mem = 0;
	TLProces x;
	void *v;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");
	while (c->ic)
	{
		ExtrQ(c, &v);
		x = (TLProces) v;
		mem += x->mem_size_in_bytes;
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
	mem += running_proces->mem_size_in_bytes;
	mem += new_process->mem_size_in_bytes;
	return mem;
}

/*facem defragmentarea, modificand, in ordine, dupa PID, adresele proceselor*/
int defragmentare(FILE *output_file, TLCoada WaitingQ, TLProces running_proces,
	TLProces new_process, char *PID, int *last_address)
{
	int i, reusita = 1;
	TLProces process;
	if (memorie_ocupata(WaitingQ, running_proces, new_process) > MAX_ADDRESS)
	{
		reusita = 0;
		fprintf(output_file, "Cannot reserve memory for PID %d.\n",
			new_process->PID);
	}
	*last_address = 0;
	for (i = 1; i <= 32767; i++)
	{
		if (PID[i] != 0)
		{
			process = search_process(i, WaitingQ, running_proces);
			if (process)
			{
				process->address = *last_address;
				*last_address += process->mem_size_in_bytes;
			}
		}
	}
	return reusita;
}


/*aici executam urmatoarele unitati de timp pe procesor*/
void run(int run_time, TLCoada WaitingQ, TLCoada FinishedQ,
         AProces running_proces, char *PID, int quantum, int *idle,
         int *remained_from_quantum)
{
	void *v = NULL;
	if (*idle == 0)
		while (run_time != 0)
		{
			/*cazul in care timpul ramas de executat devine 0*/
			if (((*running_proces)->exec_time_in_ms <= *remained_from_quantum
				&& *remained_from_quantum <= run_time)
			        || ((*running_proces)->exec_time_in_ms <= run_time &&
			        	run_time <= *remained_from_quantum))
			{
				/*modificam timpul de rulat, cat s-a executat pana atunci
				si cat a mai ramas de executat*/
				run_time -= (*running_proces)->exec_time_in_ms;
				(*running_proces)->executed_time +=
				(*running_proces)->exec_time_in_ms;
				(*running_proces)->exec_time_in_ms = 0;
				/*PID-ul procesului devine disponibil*/
				PID[(*running_proces)->PID]--;
				/*introducem procesul in coada proceselor terminate*/
				IntrQ(FinishedQ, *running_proces);
				/*resetam cuanta*/
				*remained_from_quantum = quantum;
				/*daca avem element in coada de asteptare,
				extragem urmatorul proces*/
				if (numar_elemente_coada(WaitingQ))
				{
					ExtrQ(WaitingQ, &v);
					*running_proces = (TLProces) v;
				}
				/*altfel ruleaza procesul idle*/
				else
				{
					*idle = 1;
					*running_proces = NULL;
				}
			}
			/*cazul in care cuanta se termina si trebuie resetata*/
			else
			if ((*remained_from_quantum <= (*running_proces)->exec_time_in_ms
				&& (*running_proces)->exec_time_in_ms <= run_time)
			         || (*remained_from_quantum <= run_time && run_time <=
			         	(*running_proces)->exec_time_in_ms))
			{
				/*modificam timpul ramas din cuanta, timpul de rulat,
				cat s-a executat pana atunci si cat a mai ramas de executat*/
				run_time -= *remained_from_quantum;
				(*running_proces)->executed_time += *remained_from_quantum;
				(*running_proces)->exec_time_in_ms -= *remained_from_quantum;
				*remained_from_quantum = quantum;
				/*daca avem element in coada de asteptare, extragem urmatorul
				proces si il introducem pe cel dinainte in coada de asteptare*/
				if (numar_elemente_coada(WaitingQ))
				{
					ExtrQ(WaitingQ, &v);
					inserare_asteptare(WaitingQ, *running_proces);
					*running_proces = (TLProces) v;
				}
			}
			/*cazul in care se termina timpul de rulat*/
			else
			if ((run_time <= (*running_proces)->exec_time_in_ms &&
				(*running_proces)->exec_time_in_ms <= *remained_from_quantum)
			    || (run_time <= *remained_from_quantum &&
			    *remained_from_quantum <= (*running_proces)->exec_time_in_ms))
			{
				/*modificam timpul ramas din cuanta, cat s-a executat
				pana atunci si cat a mai ramas de executat*/
				*remained_from_quantum -= run_time;
				(*running_proces)->executed_time += run_time;
				(*running_proces)->exec_time_in_ms -= run_time;
				/*daca s-a terminat cuanta*/
				if (*remained_from_quantum == 0)
				{
					/*resetam cuanta*/
					*remained_from_quantum = quantum;
					/*extragem urmatorul proces si il introducem
					pe cel dinainte in coada de asteptare*/
					ExtrQ(WaitingQ, &v);
					inserare_asteptare(WaitingQ, *running_proces);
					*running_proces = (TLProces) v;
				}
				run_time = 0;
			}
			/*daca intra procesul idle, ne oprim*/
			if (*idle == 1)
				break;
		}
}

/*calculam cat a mai ramas de executat*/
void finish(FILE *output_file, TLCoada c, TLProces running_proces)
{
	int time = 0;
	TLProces x;
	void *v;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");
	while (c->ic)
	{
		ExtrQ(c, &v);
		x = (TLProces) v;
		time += x->exec_time_in_ms;
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
	time += running_proces->exec_time_in_ms;
	fprintf(output_file, "Total time: %d\n", time);
}

/*calculam urmatoarea adresa de dupa o adresa referinta*/
int next_address(TLCoada c, TLProces running_proces, int address)
{
	int next_address = MAX_ADDRESS;
	TLProces x;
	void *v;
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");
	while (c->ic)
	{
		ExtrQ(c, &v);
		x = (TLProces) v;
		/*adresa procesului e mai mare decat adresa referinta
		iar adresa nu mai a fost modificata(<=> e mai mare ca adresa
		procesului, dupa ce o gasim, va fi mai mica)*/
		if (x->address > address && next_address > x->address)
			next_address = x->address;
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
	if (running_proces)
	{
		if (running_proces->address > address && next_address >
			running_proces->address)
			next_address = running_proces->address;
	}
	return next_address;
}

/*calculam adresa minima disponibila de memorie suficenta*/
int adresa_minima_disponibila_de_mem_suf(TLCoada c, TLProces running_proces,
	int mem_size_in_bytes)
{
	/*nu am intalnit adresa 0*/
	int adresa_0 = 0;
	int next_ad;
	int adresa_min_disp_de_mem_suf = MAX_ADDRESS;
	TLProces x;
	void *v;
	/*facem o copie cozii*/
	TLCoada copie = InitC(sizeof(TProces));
	copy_coada((void *)c, (void *)copie);
	/*initializam o coada auxiliara*/
	TLCoada caux = InitC(sizeof(TProces));
	if (!caux)
		fprintf(stderr, "Eroare Alocare");
	while (c->ic)
	{
		ExtrQ(c, &v);
		x = (TLProces) v;
		next_ad = next_address(copie, running_proces, x->address);
		/*verificam daca avem loc si daca nu depaseste memoria disponibila*/
		if (next_ad - x->address - x->mem_size_in_bytes >= mem_size_in_bytes &&
		    x->address + x->mem_size_in_bytes <= adresa_min_disp_de_mem_suf)
			adresa_min_disp_de_mem_suf = x->address + x->mem_size_in_bytes;
		/*retinem daca am intalnit adresa 0*/
		if (x->address == 0)
			adresa_0 = 1;
		IntrQ(caux, v);
	}
	while (caux->ic)
	{
		ExtrQ(caux, &v);
		IntrQ(c, v);
	}
	void *cvaux = (void *)caux;
	DistrugeC(&cvaux);
	/*procedam la fel si pentru procesul care ruleaza 
	(ca sa tratam toate procesele)*/
	if (running_proces)
	{
		next_ad = next_address(copie, running_proces, running_proces->address);
		if (next_ad - running_proces->address - 
			running_proces->mem_size_in_bytes >= mem_size_in_bytes &&
		        running_proces->address + running_proces->mem_size_in_bytes
		        <= adresa_min_disp_de_mem_suf)
			adresa_min_disp_de_mem_suf = running_proces->address +
			running_proces->mem_size_in_bytes;
		if (running_proces->address == 0)
			adresa_0 = 1;
	}
	/*tratam separat daca avem spatiu liber la inceput*/
	if (adresa_0 == 0)
	{
		next_ad = next_address(copie, running_proces, 0);
		if (next_ad >= mem_size_in_bytes)
			adresa_min_disp_de_mem_suf = 0;
	}
	void *vcopie = (void *)copie;
	DistrugeCopie (&vcopie);
	return adresa_min_disp_de_mem_suf;
}