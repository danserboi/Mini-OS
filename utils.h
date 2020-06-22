/*SERBOI Florea-Dan - Grupa 315CB*/
#define MAX_ADDRESS 3145728

typedef struct celulag
{
	/*adresa informatie*/
	void* info;
	/*adresa urmatoarei celule*/
	struct celulag *urm;
} TCelulaG, *TLG, **ALG; /*tipurile Celula, Lista si Adresa_Lista*/

typedef struct {
	size_t dime;
	TLG vf;
} TStiva, *TLStiva, **AStiva;

typedef struct proces {
	TLStiva stiva_proces;
	int PID;
	int mem_size_in_bytes;
	int exec_time_in_ms;
	int executed_time;
	int priority;
	int address;
	struct proces *urm;
} TProces, *TLProces, **AProces;

typedef struct {
	size_t dime;
	TLProces ic, sc;
} TCoada, *TLCoada, **ACoada;

TLG AlocCelula(void *ax, size_t d);

void *InitS(size_t d);

int Push(void *s, void *ae);

int Pop(void *s, void *ae);

void DistrugeS(void **aa);

void Afisare_stiva(FILE *output_file, TLStiva s);

int numar_elemente_stiva(TLStiva s);

TLProces AlocProces(int PID, int mem_size_in_bytes, int exec_time_in_ms,
                    int priority, int address);

void DezalocProces(AProces proces);

void *InitC(size_t d);

int IntrQ(void *c, void *ae);

int ExtrQ(void *c, void **ae);

int PrimQ(void *c, void *ae);

void DistrugeC(void **aa);

void DistrugeCopie(void **aa);

int copy_coada(void *c, void *new);

int numar_elemente_coada(TCoada *c);

void extract_parameters(char operation[30], char parameters[10][30], int *n);

void print_stack(FILE *output_file, TLProces process, int PID);

void print_waiting(FILE *output_file, TCoada *c);

void print_finished(FILE *output_file, TCoada *c);

int min_PID(char *PID);

void inserare_asteptare(TLCoada c, TLProces process);

void get(FILE *output_file, int PID, TLCoada WaitingQ, TLCoada FinishedQ,
	TLProces running_proces);

TLProces search_process(int PID, TLCoada WaitingQ, TLProces running_proces);

void push_op(FILE *output_file, int PID, int data, TLCoada WaitingQ,
	TLProces running_proces);

void pop_op(FILE *output_file, int PID, TLCoada WaitingQ,
	TLProces running_proces);

int memorie_ocupata(TLCoada c, TLProces running_proces, TLProces new_process);

int defragmentare(FILE *output_file, TLCoada WaitingQ, TLProces running_proces,
	TLProces new_process, char *PID, int *last_address);

void run(int run_time, TLCoada WaitingQ, TLCoada FinishedQ,
         AProces running_proces, char *PID, int quantum, int *idle,
         int *remained_from_quantum);

void finish(FILE *output_file, TLCoada c, TLProces running_proces);

int next_address(TLCoada c, TLProces running_proces, int address);

int adresa_minima_disponibila_de_mem_suf(TLCoada c, TLProces running_proces,
	int mem_size_in_bytes);