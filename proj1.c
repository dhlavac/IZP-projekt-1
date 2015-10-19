#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define MAX_LENGTH 1024 // maximalna dlzda retazca v jednej bunke

const char *HELPMSG =  // vypise napovedu 
  "Program Vypocty v tabulke.\n"
  "Autor: Dominik Hlavac Duran 2014\n"
  " Program pracuje s tabulkou a podla volby argumentu bud vypise zadany rozsah tabulky\n"
  " ,najde maximalnu alebo minimalnu hodnotu zo zadaneho rozsahu tabulky ,\n"
  " alebo vypise aritmeticky priemer zo zadaneho rozsahu tabulky \n"
  "Pouzitie: proj1 --help   vypise tuto tabulku\n"
  " proj1 select značí operaci, která  vytiskne hodnoty daných buněk\n"
  " proj1 min značí vyhledání a následný tisk minimální hodnoty z daného rozsahu buněk,\n"
  " proj1 max značí vyhledání a následný tisk maximální hodnoty z daného rozsahu buněk,\n"
  " proj1 sum značí výpočet a následný tisk sumy hodnot všech vybraných buněk,\n"
  " proj1 avg značí výpočet a následný tisk aritmetického průměru vybraných buněk.\n"
  "Rozsah je definován jedním z následujících argumentů:\n"
  " proj1 row X značí výběr všech buněk na řádku X (\n"
  " proj1 col X značí výběr všech buněk ve sloupci X \n"
  " proj1 rows X Y značí výběr všech buněk od řádku X (včetně) až po Y (včetně).\n"
  " proj1 cols X Y značí výběr všech buněk od sloupce X (včetně) až po Y (včetně). \n"
  " proj1 range A B X Y značí výběr buněk od řádku A po řádek B a od sloupce X po sloupec Y. \n"
  ;


// Udajove struktry pre operacie 
typedef enum Operation_ 
{
    select, 
    min,
    max,
    sum,
    avg
} OperationType;

typedef struct OpArgs_
{
    char *name;
    OperationType function;
    int required_parameters;
} Operation;

// Inicializacia pola s pripustnymi operaciami nad tabulkou
#define OPERATION_COUNT 5 // pocet operacii
Operation OperationArgs[OPERATION_COUNT] = {
{ "select", select, 1 },
{ "min"   , min   , 1 },
{ "max"   , max   , 1 },
{ "sum"   , sum   , 1 },
{ "avg"   , avg   , 1 }
                                           };

//Udajove struktury pre vyberove funkcie 
typedef enum Range_ 
{
    row,
    col,
    rows,
    cols,
    range
} RangeType;

typedef struct RArgs_
{
    char *name;
    RangeType function;
    int required_parameters;
} RangeFunction;

// Inicializacia pola s funkciami nad tabulkou
#define RANGES_COUNT 5 
RangeFunction RangeArgs[RANGES_COUNT] = {
{ "row"   , row   ,   1 },
{ "col"   , col   ,   1 },
{ "rows"  , rows  ,   2 },
{ "cols"  , cols  ,   2 },
{ "range" , range ,   4 }
                                        };
    
/* Funkcia vypise text helpu. */
void print_help(void){
     printf("%s", HELPMSG);;
}

/* Funkcia zisti, ci retazec v arg_option je z operacii programu. Ak ano,
  fukcia vrati EXIT_SUCCESS a *op naplni potrebnymi hodnotami 
  pre dalsie spracovanie. */
int get_operation(char *arg_operation, Operation *op)
{
    
    int i = 0;
    while(i < OPERATION_COUNT && strcmp(OperationArgs[i].name, arg_operation) != 0){
        i++;
    }

    if(i < OPERATION_COUNT)
    {
        *op = OperationArgs[i];
    }
    else
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* Funkcia zisti, ci retazec v arg_option je z pripustnych  funkcii  programu. 
  Ak ano, fukcia vrati EXIT_SUCCESS a *r naplni potrebnymi hodnotami */
int get_range(char *arg_range, RangeFunction *r)
{
    int i = 0;
    while(i < RANGES_COUNT && strcmp(RangeArgs[i].name, arg_range) != 0){
        i++;
    }
    if(i < RANGES_COUNT)
    {
        *r = RangeArgs[i];
    }
    else
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}

// funkcia rozhodne, ci dany riadok a stlpec spada do vyberu 
int in_range(int row, int col, int row_a, int row_b, int col_a, int col_b)
{
    /* Ak pride obmedzenie na riadky, skontrolujem, ci sa bunka nachadza v damon riadku */
    if(row_a != -1 && row_b != -1) 
    {
        if(row_a <= row && row <= row_b)
        {
            ; // Este je potrebne skontrolovat stlpce
        }
        else 
        {
            return FALSE;
        }
    }

    /* Ak sa bunka nachadza v zadanom riadku a prislo obmedzenie este
      na stlpce, tak skontrolujem, ci sa nachadza aj v stlpci. */
    if(col_a != -1 && col_b != -1) 
    {
        if(col_a <= col && col <= col_b)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

/* Funkcia urci, ci je retazec *val  cislo. Ak ano, funkcia vrati
  TRUE a naplni *f danym cislom. */
int is_number(char *val, double *f)
{

   if(sscanf(val, "%lf", f) != 0)
   {
       //printf("Nacitana hodnota: %.10g\n", *f);
       return TRUE;
   }
   
   return FALSE;
}

/* Funkcia vrati hodnotu bunky naplnenim  *word a nastavi *p_row, *p_col, ktore sa pouziju ako nova pozicia vo vstupe */
int get_cell_value(char *word, int *p_row, int *p_col)
{
    char c;
    if(scanf("%s", word) == EOF)
    {
        return EXIT_FAILURE;
    }
    else
    {

        //Zoberiem dalsi znak z buffera a zistim, ci to nie je koniec riadka. 
        if((c = getc(stdin)) == '\n')
        {
            (*p_row)++;
            *p_col = 1;
        }
        else
        {
            (*p_col)++; 
        }
    }
    return EXIT_SUCCESS;
}

int operation_select(char *word)
{
    double num_value;
//Zistim, ci je cislo. Ak ano, vypisem ho v pozadovanom formate. 
    if(is_number(word, &num_value))
        printf("%.10g", num_value);
    else
        printf("%s", word);

    return EXIT_SUCCESS;
}

int operation_min(char *word, double *result, int count)
{
    double num_value;
    if(is_number(word, &num_value))
    {
        //printf("%.10g\n", num_value);
        if(count == 1)
        {
            *result = num_value;
        }
        else
        {
            if(num_value < *result) 
                *result = num_value;
        }
    }
    else
    {
        fprintf(stderr,"ERROR, hodnota nieje cislo\n");
       return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int operation_max(char *word, double *result, int count)
{
    double num_value;
    if(is_number(word, &num_value))
    {
        //printf("%.10g\n", num_value);
        if(count == 1)
        {
            *result = num_value;
        }
        else
        {
            if(num_value > *result) 
                *result = num_value;
        }
    }
    else
    {
       fprintf(stderr,"ERROR, hodnota nieje cislo\n");
       return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* operacia sum */
int operation_sum(char *word, double *result)
{
    double num_value;
    if(is_number(word, &num_value))
    {
        //printf("%.10g\n", num_value);
        *result += num_value;
    }
    else
    {
       fprintf(stderr,"ERROR, hodnota nieje cislo\n");
       return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    Operation operation; // operacie na vstupe, napr. select, min, max ...
    RangeFunction range_function; // range funkcia na vstupe, napr. row, col, ...
    int row_y0, row_y1, col_x0, col_x1 = -1; // pomocne premenne pre vyberove funkcie
    double double_tmp; // pomocna premenna pre typ double
    char word[MAX_LENGTH]; // obsah bunky
    int act_row = 1; // riadok, na ktorom sa momentalne nechadzam
    int act_col = 1; // stlpec, na ktorom sa momentalne nechadzam

    double result = 0.0; // priebezny a celkovy vysledok matematickych funkcii
    int count = 0; // pocet spracovanych cisel v matematickych funkciach
    int cell_in_area = FALSE; // kontrola ci sa nachadzam v zadanej oblasti


    if((strcmp("--help", argv[1]) == 0))  /* Ak je zadany parameter -- help tak vypise napovedu */
    {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
    }
    /* Spracovanie vstupnych parametrov */
    if(argc == 1)
    {
        fprintf(stderr,"Nebol zadany ziadny argument, pre napovedu spustite program v tvare ./proj1 -- help\n"); // ak uzivatel nezada ziadny parameter tak mu vypise hlasenie
        return EXIT_FAILURE;
    }

    if (argc > 7)
      fprintf(stderr,"CHYBA zadal si prilis mnoho argumentov \n");

       // Operacia select, min, max ...
    if(get_operation(argv[1], &operation) == EXIT_FAILURE){
         fprintf(stderr,"ERROR, pre napovedu spustite program v tvare ./proj1 -- help\n");
        return EXIT_FAILURE;
    }
    if(argc < 2 + operation.required_parameters)
    {
        fprintf(stderr,"ERROR: Malo paramterov pre operaciu \n");
        return EXIT_FAILURE;
    }

    // Rozsah/vyber, napr. row, rows, col, ...
    if(get_range(argv[2], &range_function) == EXIT_FAILURE){
        fprintf(stderr,"ERROR \n");
        return EXIT_FAILURE;
    }


    if(argc < 3 + range_function.required_parameters)
    {
        printf("ERROR: Malo argumentov pre vyberovu funkciu \n");
        return EXIT_FAILURE;
    }
    /* Osetrenie vyberovych funkcii.
     Ak je pre vyberovu funkciu pozuitych vela argumentov, zoberiem prvy a ostatne ignorujem (napr.: `row 4 5', do row pojde 4 a 5 ignorujem). */
    switch(range_function.function)
    {
        case row:
            if(is_number(argv[3], &double_tmp) && (int) double_tmp > 0)
            {
                row_y0 = (int) double_tmp;
                row_y1 = row_y0;
                col_x0 = -1;
                col_x1 = -1;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            break;
        case col:
            if(is_number(argv[3], &double_tmp) && (int) double_tmp > 0)
            {
                row_y0 = -1;
                row_y1 = -1;
                col_x0 = (int) double_tmp;
                col_x1 = col_x0;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            break;
        case rows:
            col_x0 = -1;
            col_x1 = -1;
            if(is_number(argv[3], &double_tmp) && (int) double_tmp > 0)
            {
                row_y0 = (int) double_tmp;
            }
            else
            {
               fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            if(is_number(argv[4], &double_tmp) && (int) double_tmp > 0)
            {
                row_y1 = (int) double_tmp;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            break;
        case cols:
            row_y0 = -1;
            row_y1 = -1;
            if(is_number(argv[3], &double_tmp) && (int) double_tmp > 0)
            {
                col_x0 = (int) double_tmp;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            if(is_number(argv[4], &double_tmp) && (int) double_tmp > 0)
            {
                col_x1 = (int) double_tmp;
            }
            else
            {
               fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            break;
        case range:
            if(is_number(argv[3], &double_tmp) && (int) double_tmp > 0)
            {
                row_y0 = (int) double_tmp;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            if(is_number(argv[4], &double_tmp) && (int) double_tmp > 0)
            {
                row_y1 = (int) double_tmp;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            if(is_number(argv[5], &double_tmp) && (int) double_tmp > 0)
            {
                col_x0 = (int) double_tmp;
            }
            else
            {
               fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
               return EXIT_FAILURE;
            }
            if(is_number(argv[6], &double_tmp) && (int) double_tmp > 0)
            {
                col_x1 = (int) double_tmp;
            }
            else
            {
                fprintf(stderr,"ERROR: Argumenty vyberovych funkcii musia byt kladne cele cisla okrem nuly!\n");
                return EXIT_FAILURE;
            }
            break;
        default:
            fprintf(stderr,"ERROR neznama akcias\n");
            return EXIT_FAILURE;
    }

    // Kontrola, ci su hranice rozsahov vyberovych funkcii OK
    if(row_y1 < row_y0)
    {
        fprintf(stderr,"ERROR:Index riadka Y musi byt vacsi ako index riadka X.\n");
        return EXIT_FAILURE;
    }
    
    if(col_x1 < col_x0)
    {
        fprintf(stderr,"ERROR:Index stlpca B musi byt vacsi ako index stlpca A.\n");
        return EXIT_FAILURE;
    }

    /* Spracovanie stdin.  Najprv zistim,ci sa bunka nachadza v pozadovanej oblasti a nasledne si nacitam jej obsah. 
     Citam bunku aj ked sa nenachadza v pozadovanej oblasti */
    while((cell_in_area = in_range(act_row, act_col, row_y0, row_y1, col_x0, col_x1), 
                get_cell_value(word, &act_row, &act_col) != EXIT_FAILURE))
    {
        if(cell_in_area) // ak sa bunka nachadza v pozadovanej oblasti, pokracujem v spracovani.
        {
            switch(operation.function)
            {
                case select:
                    if(operation_select(word) == EXIT_SUCCESS)
                        printf("\n"); 
                    else
                        return EXIT_FAILURE;
                    break;

                case min:
                    count++;
                    if(operation_min(word, &result, count) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                    break;

                case max:
                    count++;
                    if(operation_max(word, &result, count) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                    break;

                case sum:
                    if(operation_sum(word, &result) == EXIT_FAILURE)
                        return EXIT_FAILURE;
                    break;

                case avg:
                    count++;
                    if(operation_sum(word, &result) == EXIT_FAILURE)
                       return EXIT_FAILURE;
                    break;

                default:
                   fprintf(stderr,"ERROR:Neznama operacia\n");
                   return EXIT_FAILURE;
           }
        }

        // Ak dosiahnem hornu hranicu riadkov, koncim spracovanie.
        if(row_y1 != -1 && row_y1 < act_row){
            break;
        }
    }
    // Osetrenie vynimok pri vypise vysledku.
    switch(operation.function)
    {
        case select:
            // Nerobim nic, len aby som sa nedostal do default
            break;

        case avg:
            result /= (double) count;

        default:
            printf("%.10g", result);
            printf("\n");
    }
    return EXIT_SUCCESS;
}
