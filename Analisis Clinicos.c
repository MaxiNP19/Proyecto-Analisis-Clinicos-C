#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int dia;
    int mes;
    int anio;
} fecha;

typedef struct {
    int protocolo;
    char paciente [50];
    char medico [50];
    int tieneOS;
    char obraSocial [50];
    int montoOS;
    int montoPaciente;
    fecha fechaAnalisis;
} cobro;

typedef struct Tnodo{
    cobro info;
    struct Tnodo *next;
} nodo;

fecha leerFecha();
void cargarInforme(nodo **c, fecha f);
void modificarInforme(nodo **c);
void eliminarInforme(nodo **h);
void buscarProtocolo(nodo *c);
void listarInformes(nodo *c);
void listarMontosOS(nodo *c);
void myflush ( FILE *in );
void mypause ( void );
int sumaMontoOS(nodo *c, char nOS[20]);

FILE *archivo;

int main () {
    int opcion;
    char filename[20];
    fecha fechaActual;
    fechaActual = leerFecha();
    printf("\nLa fecha ingresada es: %d/%d/%d\n\n", fechaActual.dia, fechaActual.mes, fechaActual.anio);
    nodo *head = malloc(sizeof(nodo));
    nodo *cursor = head->next;
    printf("Ingrese el nombre del archivo (sin extension): ");
    scanf("%s", filename);
    strcat(filename, ".dat");
    archivo = fopen(filename, "rb");

    if (NULL == archivo) {
        printf("\nEl archivo especificado no existe. Sera creado automaticamente.\n");
        sleep(3);
        cursor = head;
    }
    else {
        printf("\nCargando archivo %s\n", filename);
        sleep(3);
        cursor = head;
        while (!feof(archivo)) {
            nodo *nuevo = malloc(sizeof(nodo));
            fread(&nuevo->info, sizeof(cobro), 1, archivo);
            if (feof(archivo)) {
                free (nuevo);
                break;
            }
            cursor->next = nuevo;
            cursor = cursor->next;
        }
        fclose(archivo);
    }

    do
	{
		system ("clear");
		printf ("MENU DE OPCIONES:\n\n");
		printf ("1. Cargar Informe\n");
		printf ("2. Modificar Informe\n");
		printf ("3. Eliminar Informe\n");
		printf ("4. Buscar un Protocolo\n");
		printf ("5. Listar Informes\n");
		printf ("6. Listar Montos OS por mes\n");
        printf ("7. Guardar y Salir\n");
        printf ("Elige una opcion: ");
		scanf ("%d", &opcion);

		switch (opcion)
		{
			case 1:
				system ("clear");
                cursor = head;
                cargarInforme(&cursor, fechaActual);
				break;
			case 2:
				system ("clear");
                cursor = head;
                modificarInforme(&cursor);
				break;
			case 3:
				system ("clear");
                eliminarInforme(&head);
				break;
            case 4:
                system ("clear");
                cursor = head->next;
                buscarProtocolo(cursor);
                break;
            case 5:
                system ("clear");
                cursor = head->next;
                listarInformes(cursor);
                break;
			case 6:
				system ("clear");
                cursor = head->next;
                listarMontosOS(cursor);
				break;
			case 7:
				system ("clear");
                cursor = head->next;
                printf("Con que nombre quiere guardar el archivo?? (Sin extension): ");
                scanf("%s", filename);
                strcat(filename, ".dat");
                archivo = fopen(filename, "wb");
                while (cursor != NULL) {
                    fwrite(&cursor->info, sizeof(cobro), 1, archivo);
                    cursor = cursor->next;
                }
                fclose(archivo);
				break;
			default:
				system ("clear");
				printf ("La opcion ingresada no es correcta. Por favor intente nuevamente.");
				break;
		};
	}while (opcion != 7);

}
    
    

fecha leerFecha() {
    fecha f;
    printf("Ingrese la fecha actual (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &f.dia, &f.mes, &f.anio);
    return f;
}

void cargarInforme(nodo **c, fecha f) {
    nodo *aux = malloc(sizeof(nodo));
    aux->next = NULL;
    while ((*c)->next != NULL) {
        (*c) = (*c)->next;
    }
    aux->info.protocolo = (*c)->info.protocolo + 1;
    printf("Ingrese el nombre del paciente: ");
    myflush(stdin);
    scanf("%[^\n]s", aux->info.paciente);
    myflush(stdin);
    printf("Ingrese el nombre del medico: ");
    scanf("%[^\n]s", aux->info.medico);
    printf("Tiene obra social? (1 = si, 0 = no): ");
    scanf("%d", &aux->info.tieneOS);
    if (aux->info.tieneOS == 1) {
        printf("Ingrese el nombre de la obra social: ");
        myflush(stdin);
        scanf("%[^\n]s", aux->info.obraSocial);
        printf("Ingrese el monto de la obra social: ");
        scanf("%d", &aux->info.montoOS);
    } else {
        strcpy (aux->info.obraSocial, "No tiene");
        aux->info.montoOS = 0;
    }
    printf("Ingrese el monto a pagar por el paciente: ");
    scanf("%d", &aux->info.montoPaciente);
    aux->info.fechaAnalisis = f;
    (*c)->next = aux;
}

void modificarInforme(nodo **c) {
    int protocolo;
    int o;
    nodo *aux = *c;
    if ((*c)->next == NULL)
    {
        printf("\nNo hay informes cargados.\n");
        myflush(stdin);
        mypause();
        return;
    }
    printf("Ingrese el protocolo del informe a modificar: ");
    scanf("%d", &protocolo);
    while (aux->next != NULL) {
        aux = aux->next;
    }
    if (aux->info.protocolo < protocolo) {
        printf("No existe el protocolo ingresado. Intente nuevamente\n");
        myflush(stdin);
        mypause();
        return;
    }
    while (c != NULL) {
        if ((*c)->info.protocolo == protocolo) {
            do {
                system ("clear");
                printf("El informe que deseas modificar es el siguiente:\n");
                printf("Protocolo: %d\n", (*c)->info.protocolo);
                printf("Paciente: %s\n", (*c)->info.paciente);
                printf("Medico: %s\n", (*c)->info.medico);
                printf("Tiene OS: %d\n", (*c)->info.tieneOS);
                printf("OS: %s\n", (*c)->info.obraSocial);
                printf("Monto OS: %d\n", (*c)->info.montoOS);
                printf("Monto Paciente: %d\n", (*c)->info.montoPaciente);
                printf("Fecha Analisis: %d/%d/%d\n\n", (*c)->info.fechaAnalisis.dia, (*c)->info.fechaAnalisis.mes, (*c)->info.fechaAnalisis.anio);
                
		        printf ("QUE CAMPO DESEA MODIFICAR? (NO PUEDE MODIFICAR FECHA NI NUMERO DE PROTOCOLO):\n\n");
		        printf ("1. Nombre del Paciente\n");
		        printf ("2. Nombre del Medico\n");
		        printf ("3. Estado de Obra Social (Tiene OS o no, Nombre y Monto a pagar)\n");
		        printf ("4. Monto a Pagar del Paciente\n");
                printf ("5. No deseo modificar mas, Guardar y Salir\n");
                printf ("Elige una opcion: ");
		        scanf ("%d", &o);

                switch (o) {
                    case 1:
                        system ("clear");
                        printf("Ingrese el nuevo nombre del paciente: ");
                        myflush(stdin);
                        scanf("%[^\n]s", &(*c)->info.paciente[0]);
                        break;
                    case 2:
                        system ("clear");
                        printf("Ingrese el nuevo nombre del medico: ");
                        myflush(stdin);
                        scanf("%[^\n]s", &(*c)->info.medico[0]);
                        break;
                    case 3:
                        system ("clear");
                        printf("Quiere cambiar status de obra social? (1 = si, 0 = no): ");
                        scanf("%d", &(*c)->info.tieneOS);
                        if ((*c)->info.tieneOS == 1) {
                            printf("Ingrese el nuevo nombre de la obra social: ");
                            myflush(stdin);
                            scanf("%[^\n]s", &(*c)->info.obraSocial[0]);
                            printf("Ingrese el nuevo monto de la obra social: ");
                            scanf("%d", &(*c)->info.montoOS);
                        } else {
                            strcpy ((*c)->info.obraSocial, "No tiene");
                            (*c)->info.montoOS = 0;
                        }
                        break;
                    case 4:
                        system ("clear");
                        printf("Ingrese el nuevo monto a pagar por el paciente: ");
                        scanf("%d", &(*c)->info.montoPaciente);
                        break;
                    case 5:
                        system ("clear");
                        break;
                    default:
                        system ("clear");
                        printf ("La opcion ingresada no es correcta. Por favor intente nuevamente.");
                        break;
                }
            }while (o != 5);
            break;
        }
        (*c) = (*c)->next;
    }
}

void eliminarInforme(nodo **h) {
    int protocolo;
    nodo *aux2 = *h;
    nodo *aux = *h;
    nodo *prev = *h;
    if ((*h)->next == NULL)
    {
        printf("\nNo hay informes cargados.\n");
        myflush(stdin);
        mypause();
        return;
    }
    printf("Ingrese el protocolo del informe a eliminar: ");
    scanf("%d", &protocolo);
    while (aux2->next != NULL) {
        aux2 = aux2->next;
    }
    if (aux2->info.protocolo < protocolo) {
        printf("No existe el protocolo ingresado. Intente nuevamente\n");
        myflush(stdin);
        mypause();
        return;
    }
    while (aux != NULL) {
        if (aux->info.protocolo == protocolo) {
            if (aux == *h) {
                *h = aux->next;
            } else {
                prev->next = aux->next;
            }
            free(aux);
            break;
        }
        prev = aux;
        aux = aux->next;
    }
}

void listarInformes(nodo *c) {    
    char tieneOS[3];
    if (c == NULL)
    {
        printf("\nNo hay informes cargados.\n");
        myflush(stdin);
        mypause();
        return;
    }
    while (c != NULL) {
        printf("Protocolo: %d\n", c->info.protocolo);
        printf("Paciente: %s\n", c->info.paciente);
        printf("Medico: %s\n", c->info.medico);
        if (c->info.tieneOS == 1) {
            strcpy(tieneOS, "Si");
        } else {
            strcpy(tieneOS, "No");
        }
        printf("Tiene OS: %s\n", tieneOS);
        printf("OS: %s\n", c->info.obraSocial);
        printf("Monto OS: %d\n", c->info.montoOS);
        printf("Monto Paciente: %d\n", c->info.montoPaciente);
        printf("Fecha Analisis: %d/%d/%d\n\n", c->info.fechaAnalisis.dia, c->info.fechaAnalisis.mes, c->info.fechaAnalisis.anio);
        c = c->next;
    }
    myflush(stdin);
    mypause();
}

void listarMontosOS(nodo *c) {
    int mes;
    int anio;
    typedef struct {
        char nombreOS[20];
        int montoTotal;
    } os;
    typedef struct {
        os o [100];
        int cantOS;
    } reg;
    reg r;
    r.cantOS = 0;
    for (int h = 0; h < 100; h++)
    {
        r.o[h].montoTotal = 0;
        strcpy(r.o[h].nombreOS, "");
    }
    
    int i = 0;
    if (c == NULL)
    {
        printf("\nNo hay informes cargados.\n");
        myflush(stdin);
        mypause();
        return;
    }

    printf("Ingrese el mes a consultar (1-12): ");
    scanf("%d", &mes);
    printf("Ingrese el año a consultar: ");
    scanf("%d", &anio);

    while (c != NULL) {
        if (c->info.tieneOS == 1 && c->info.fechaAnalisis.mes == mes && c->info.fechaAnalisis.anio == anio) {
            while (i < r.cantOS && strcmp(r.o[i].nombreOS, c->info.obraSocial) != 0)
            {
                i++;
            }
            if (r.o[i].montoTotal == 0)
            {
                strcpy(r.o[i].nombreOS, c->info.obraSocial);
                r.o[i].montoTotal = sumaMontoOS(c, r.o[i].nombreOS);
                r.cantOS++;
            }
        }
        c = c->next;
    }

    if (r.cantOS == 0)
    {
        printf("No hay pagos de Obras Sociales para el mes ingresado.\n");
        myflush(stdin);
        mypause();
        return;
    }
    printf("Los montos totales a pagar por las obras sociales en %d/%d son:\n\n", mes, anio);
    for (int j = 0; j < r.cantOS; j++)
    {
        printf("%s: %d\n\n", r.o[j].nombreOS, r.o[j].montoTotal);
    }
    myflush(stdin);
    mypause();
   
}


int sumaMontoOS(nodo *c, char nOS[20]) {
    if (c == NULL){
        return 0;
    }
    else if ((c->info.tieneOS == 1)){
        if ((strcmp(c->info.obraSocial, nOS) == 0)){
            return sumaMontoOS(c->next, nOS) + c->info.montoOS;
        }
        else{
            return sumaMontoOS(c->next, nOS);
        }
    }
}

void myflush ( FILE *in )
{
  int ch;

  do
    ch = fgetc ( in ); 
  while ( ch != EOF && ch != '\n' ); 

  clearerr ( in );
}

void mypause ( void ) 
{ 
  printf ( "\nApriete Enter para continuar . . ." );
  fflush ( stdout );
  getchar();
} 

void buscarProtocolo(nodo *c) {
    cobro arr [1000];
    int i = 0;
    int protocolo;
    int k = 0;
    int inf = 0;
    int sup;
    nodo *aux = c;
    if (c == NULL)
    {
        printf("\nNo hay informes cargados.\n");
        myflush(stdin);
        mypause();
        return;
    }
    printf("Ingrese el protocolo a buscar: ");
    scanf("%d", &protocolo);
    while (aux->next != NULL) {
        aux = aux->next;
    }
    if (aux->info.protocolo < protocolo) {
        printf("No existe el protocolo ingresado. Intente nuevamente\n");
        myflush(stdin);
        mypause();
        return;
    }

    while (c != NULL) {
        arr[i] = c->info;
        c = c->next;
        i++;
    }
    sup = i;
    while (inf <= sup) {
        k = (inf + sup) / 2;
        if (arr[k].protocolo == protocolo) {
            printf("\nProtocolo: %d\n", arr[k].protocolo);
            printf("Paciente: %s\n", arr[k].paciente);
            printf("Medico: %s\n", arr[k].medico);
            printf("Tiene OS: %d\n", arr[k].tieneOS);
            printf("OS: %s\n", arr[k].obraSocial);
            printf("Monto OS: %d\n", arr[k].montoOS);
            printf("Monto Paciente: %d\n", arr[k].montoPaciente);
            printf("Fecha Analisis: %d/%d/%d\n\n", arr[k].fechaAnalisis.dia, arr[k].fechaAnalisis.mes, arr[k].fechaAnalisis.anio);
            myflush(stdin);
            mypause();
            break;
        } else if (arr[k].protocolo < protocolo) {
            inf = k + 1;
        } else {
            sup = k - 1;
        }
    }



}
