/* Implementação do código de teste para o código da fila */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

/* Nosso programa precisa utilizar regularmente malloc/free */
#define INTERNAL 1
#include "harness.h"

/*
  Este é um pequeno rascunho ao utilizar este arquivo #include  na versão de solução do código

  OK as long as head field of queue_t structure is in first position in solution code
*/
#include "queue.h"

#include "report.h"
#include "console.h"

/***** Parâmetros Configuráveis *****/

/*
  Tamanho da fila antes dela ser considerada grande.
  Isto afeta como ela é impressa
  e quais modos são utilizados para liberar a lista
*/
#define BIG_QUEUE 30

int big_queue_size = BIG_QUEUE;

/******* Variáveis Globais ******/

/* Fila sendo testada */
queue_t *q = NULL;
/* Número de elementos na fila */
size_t qcnt = 0;

/* Quantas vezes as operações na fila pode falhar */
int fail_limit = BIG_QUEUE;
int fail_count = 0;

/****** Forward declarations ******/
static bool show_queue(int vlevel);
bool do_new(int argc, char *argv[]);
bool do_free(int argc, char *argv[]);
bool do_insert_head(int argc, char *argv[]);
bool do_insert_tail(int argc, char *argv[]);
bool do_remove_head(int argc, char *argv[]);
bool do_remove_head_quiet(int argc, char *argv[]);
bool do_reverse(int argc, char *argv[]);
bool do_size(int argc, char *argv[]);
bool do_show(int argc, char *argv[]);

static void queue_init();

static void console_init() {
    add_cmd("new", do_new,
	    "                | Cria uma nova fila");
    add_cmd("free", do_free,
	    "                | Remove a fila");
    add_cmd("ih", do_insert_head,
	    " v [n]          | Insere v na cabeça da fila n vezes (default: n == 1)");
    add_cmd("it", do_insert_tail,
	    " v [n]          | Insere v na calda da fila n vezes (default: n == 1)");
    add_cmd("rh", do_remove_head,
	    " [v]            | Remove da cabeça da fila.  Opcionalmente compara ao valor experado de v");
    add_cmd("rhq", do_remove_head_quiet,
	    " [v]            | Remove da cabeça da fila sem mostrar o valor");
    add_cmd("reverse", do_reverse,
	    "                | Inverte a fila");
    add_cmd("size", do_size,
	    " [n]            | Compute queue size n times (default: n == 1)");
    add_cmd("show", do_show,
	    "                | Mostra o conteúdo da fila");
    add_param("malloc", &fail_probability, "Percentual de probabilidade de falha do Malloc", NULL);
    add_param("fail", &fail_limit, "Numero de vezes que as operações na fila retornou falso", NULL);
}

bool do_new(int argc, char *argv[])
{
    bool ok = true;
    if (q != NULL) {
	     report(3, "Liberando a velha fila");
	      ok = do_free(argc, argv);
    }
    error_check();
    if (exception_setup(true))
      q = q_new();
    exception_cancel();
    qcnt = 0;
    show_queue(3);
    return ok && !error_check();
}

bool do_free(int argc, char *argv[])
{
    bool ok = true;
    if (q == NULL)
	report(3, "Warning: Calling free on null queue");
    error_check();
    if (qcnt > big_queue_size)
	set_cautious_mode(false);
    if (exception_setup(true))
	q_free(q);
    exception_cancel();
    set_cautious_mode(true);
    q = NULL;
    qcnt = 0;
    show_queue(3);
    size_t bcnt = allocation_check();
    if (bcnt > 0) {
	report(1, "ERROR: Freed queue, but %lu blocks are still allocated", bcnt);
	ok = false;
    }
    return ok && !error_check();
}

bool do_insert_head(int argc, char *argv[])
{
    int val;
    int reps = 1;
    int r;
    bool ok = true;
    if (argc != 2 && argc != 3) {
	report(1, "%s precisa de 1-2 argumentos", argv[0]);
	return false;
    }
    if (!get_int(argv[1], &val)) {
	report(1, "Valor de insercao invalido '%s'", argv[1]);
	return false;
    }
    if (argc == 3) {
	if (!get_int(argv[2], &reps)) {
	    report(1, "Numero de insercoes invalido '%s'", argv[2]);
	    return false;
	}
    }
    if (q == NULL)
	report(3, "Atencao: Chamando uma insercao na cabeca sobre uma fila nula");
    error_check();
    if (exception_setup(true)) {
	for (r = 0; ok && r < reps; r++) {
	    bool rval = q_insert_head(q, val);
	    if (rval) {
		qcnt++;
	    } else {
		fail_count++;
		if (fail_count < fail_limit)
		    report(2, "Insercao de %d falhou", val);
		else {
		    report(1, "ERROR: Insercao de %d falhou (%d falhas totais)", val, fail_count);
		    ok = false;
		}
	    }
	    ok = ok &&!error_check();
	}
    }
    exception_cancel();
    show_queue(3);
    return ok;
}

bool do_insert_tail(int argc, char *argv[])
{
    int val;
    int reps = 1;
    int r;
    bool ok = true;
    if (argc != 2 && argc != 3) {
	report(1, "%s precisa de 1-2 argumentos", argv[0]);
	return false;
    }
    if (!get_int(argv[1], &val)) {
	report(1, "Valor de insercao invalido '%s'", argv[1]);
	return false;
    }
    if (argc == 3) {
	if (!get_int(argv[2], &reps)) {
	    report(1, "Numero de insercoes invalido '%s'", argv[2]);
	    return false;
	}
    }
    if (q == NULL)
	report(3, "Atencao: Chamando uma insercao na calda sobre uma fila nula");
    error_check();
    if (exception_setup(true)) {
	for (r = 0; ok && r < reps; r++) {
	    bool rval = q_insert_tail(q, val);
	    if (rval) {
		qcnt ++;
	    } else {
		fail_count++;
		if (fail_count < fail_limit)
		    report(2, "Insercao de %d falhou", val);
		else {
		    report(1, "ERROR: Insercao de %d falhou (%d falhas totais)", val, fail_count);
		    ok = false;
		}
	    }
	    ok = ok && !error_check();
	}
    }
    exception_cancel();
    show_queue(3);
    return ok;
}

bool do_remove_head(int argc, char *argv[])
{
    int val, ival, eval;
    bool check = argc > 1;
    bool ok = true;
    val = ival = random();
    if (check) {
	if (!get_int(argv[1], &eval)) {
	    report(1, "Valor de comparacao invalido '%s'", argv[1]);
	    return false;
	}
    }
    if (q == NULL)
	report(3, "Atencao: Chamando remocao na cabeca sobre uma fila nula");
    else if (q->head == NULL)
	report(3, "Atencao: Chamando remocao na cabeca sobre uma fila vazia");
    error_check();
    bool rval = false;
    if (exception_setup(true))
	rval = q_remove_head(q, &val);
    exception_cancel();
    if (rval) {
	if (val == ival) {
	    report(1, "ERROR:  Falha ao armazena o valor removido");
	    ok = false;
	} else {
	    report(2, "Removido %d da fila", val);
	}
	qcnt--;
    } else {
	fail_count++;
	if (!check && fail_count < fail_limit)  {
	    report(2, "Remocao da fila falhou");
	} else {
	    report(1, "ERROR:  Remocao da fila falhou (%d falhas totais)", fail_count);
	    ok = false;
	}
    }
    if (ok && check && val != eval) {
	report(1, "ERROR:  Valor removido %d != valor esperado %d", val, eval);
	ok = false;
    }
    show_queue(3);
    return ok && !error_check();
}

bool do_remove_head_quiet(int argc, char *argv[])
{
    bool ok = true;
    if (q == NULL)
	report(3, "Atencao: Chamando remover da cabeca sobre uma fila nula");
    else if (q->head == NULL)
	report(3, "Atencao: Chamando remover da cabeca sobre uma fila vazia");
    error_check();
    bool rval = false;
    if (exception_setup(true))
	rval = q_remove_head(q, NULL);
    exception_cancel();
    if (rval) {
	report(2, "Removido o elemento da fila");
	qcnt--;
    } else {
	fail_count++;
	if (fail_count < fail_limit)
	    report(2, "Falha na remocao");
	else {
	    report(1, "ERROR: Remocao falhou (%d falhas totais)", fail_count);
	    ok = false;
	}
    }
    show_queue(3);
    return ok && !error_check();
}

bool do_reverse(int argc, char *argv[])
{
    if (q == NULL)
	report(3, "Atencao: Chamando reverso sobre uma fila nula");
    error_check();
    if (exception_setup(true))
	q_reverse(q);
    exception_cancel();
    show_queue(3);
    return !error_check();
}

bool do_size(int argc, char *argv[])
{

    int reps = 1;
    int r;
    bool ok = true;
    if (argc != 1 && argc != 2) {
	report(1, "%s precisa 0-1 argumentos", argv[0]);
	return false;
    }
    if (argc == 2) {
	if (!get_int(argv[1], &reps)) {
	    report(1, "Numero invalido de chamadas para o tamanho '%s'", argv[2]);
	}
    }
    int cnt = 0;
    if (q == NULL)
	report(3, "Atencao: Chamando tamanho sobre uma fila nula");
    error_check();
    if (exception_setup(true)) {
	for (r = 0; ok && r < reps; r++) {
	    cnt = q_size(q);
	    ok = ok && !error_check();
	}
    }
    exception_cancel();
    if (ok) {
	if (qcnt == cnt) {
	    report(2, "Tamanho da fila = %d", cnt);
	} else {
	    report(1, "ERROR:  Tamanho da fila computado como %d, mas o correto e %d",
		   cnt, (int) qcnt);
	    ok = false;
	}
    }
    show_queue(3);

    return ok && !error_check();
}

static bool show_queue(int vlevel)
{
    bool ok = true;
    if (verblevel < vlevel)
	return true;
    int cnt = 0;
    if (q == NULL) {
	report(vlevel, "q = NULL");
	return true;
    }
    report_noreturn(vlevel, "q = [");
    list_ele_t *e = q->head;
    if (exception_setup(true)) {
	while (ok && e && cnt < qcnt) {
	    if (cnt < big_queue_size)
		report_noreturn(vlevel, cnt == 0 ? "%d" : " %d", e->value);
	    e = e->next;
	    cnt++;
	    ok = ok && !error_check();
	}
    }
    exception_cancel();
    if (!ok) {
	report(vlevel, " ... ]");
	return false;
    }
    if (e == NULL) {
	if (cnt <= big_queue_size)
	    report(vlevel, "]");
	else
	    report(vlevel, " ... ]");
    } else {
	report(vlevel, " ... ]");
	report(vlevel, "ERROR:  Ou a lista possui ciclos, ou a fila possui mais que %d elementos",
	       qcnt);
	ok = false;
    }
    return ok;
}

bool do_show(int argc, char *argv[])
{
    return show_queue(0);
}

/* Manipuladores de sinal */
void sigsegvhandler(int sig) {
    trigger_exception("Ocorreu uma falta de segmentacao. Voce derefenciou um NULL ou ponteiro invalido");
}

void sigalrmhandler(int sig) {
    trigger_exception("Limite de tempo execedido. Ou voce está num loop infinito, ou seu codigo e muito ineficiente");
}


static void queue_init() {
    fail_count = 0;
    q = NULL;
    signal(SIGSEGV, sigsegvhandler);
    signal(SIGALRM, sigalrmhandler);
}


static bool queue_quit(int argc, char *argv[]) {
    report(3, "Liberando fila");
    if (qcnt > big_queue_size)
	set_cautious_mode(false);
    if (exception_setup(true))
	q_free(q);
    exception_cancel();
    set_cautious_mode(true);
    size_t bcnt = allocation_check();
    if (bcnt > 0) {
	report(1, "ERROR: Fila liberada, mas %lu blocos estao ainda alocados", bcnt);
	return false;
    }
    return true;
}


static void usage(char *cmd) {
    printf("Utilize: %s [-h] [-f IFILE][-v VLEVEL][-l LFILE]\n",  cmd);
    printf("\t-h         Imprime esta informacao\n");
    printf("\t-f IFILE   Le comandos de IFILE\n");
    printf("\t-v VLEVEL  Configura o nivel de relatorio\n");
    printf("\t-l LFILE   Mostras os resultados no arquivo LFILE\n");
    exit(0);
}

#define BUFSIZE 256

int main(int argc, char *argv[]) {
    /* Para manter o nome do arquivo de entrada */
    char buf[BUFSIZE];
    char *infile_name = NULL;
    char lbuf[BUFSIZE];
    char *logfile_name = NULL;
    int level = 4;
    int c;

    while ((c = getopt(argc, argv, "hv:f:l:")) != -1) {
	switch(c) {
	case 'h':
	    usage(argv[0]);
	    break;
	case 'f':
	    infile_name = strncpy(buf, optarg, BUFSIZE-1);
	    buf[BUFSIZE-1] = '\0';
	    break;
	case 'v':
	    level = atoi(optarg);
	    break;
	case 'l':
	    logfile_name = strncpy(lbuf, optarg, BUFSIZE-1);
	    lbuf[BUFSIZE-1] = '\0';
	    break;
	default:
	    printf("Opcao desconhecida option '%c'\n", c);
	    usage(argv[0]);
	    break;
	}
    }
    queue_init();
    init_cmd();
    console_init();
    set_verblevel(level);
    if (level > 1) {
	set_echo(true);
    }
    if (logfile_name)
	set_logfile(logfile_name);
    add_quit_helper(queue_quit);
    bool ok = true;
    ok = ok && run_console(infile_name);
    ok = ok && finish_cmd();
    return ok ? 0 : 1;
}
