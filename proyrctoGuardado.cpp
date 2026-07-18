#include <string.h>
#include <iostream>
using namespace std;
//LUIS LARA Y GABRIEL FREAY


struct Sventa {
    int num_operacion;
    int codigo_producto;     
    int codigo_asociado;   
    int cantidad;
    float precio_unidad;
    float monto_total;
    int fecha;               
    
    struct Sventa* prventas; 
};

struct Sasociado {
    int codigo;              
    char nombre[50];
    char direccion[100];
    char telefono[21]; 
    
    struct Sasociado* pnext; 
    struct Sventa* pventas;  
};

struct Sproducto {
    int codigo;
    char nombre[30];
    char marca[30];
    char descripcion[100];
    Sproducto* psig;
};

bool BuscarCodigoProducto(Sproducto *lista, int codigo){
    if(!(lista)){
        return true;
    }
    else{
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->psig;}
        }
        return true;
    }
}

bool BuscarCodigoAsociado(Sasociado *lista, int codigo){
    if(!(lista)){
        return true;
    }
    else{
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->pnext;}
        }
        return true;
    }
}

void ToLower(const char origen[], char destino[]) {
    size_t i = 0;
    while (origen[i] != '\0') {
        char c = origen[i];
        if (c >= 'A' && c <= 'Z') {
            destino[i] = c - 'A' + 'a';
        } else {
            destino[i] = c;
        }
        ++i;
    }
    destino[i] = '\0';
}

bool IsNumeric(const char origen []){
    int i = 0;
    while (origen[i] != '\0'){
        char c = origen[i];
        if(!isdigit(c)){
            return false;
        }
        i++;
    }
    return true;
}

static int ContainsIgnoreCase(const char texto[], const char patron[]) {
    char textoMinusculas[200];
    char patronMinusculas[200];
    ToLower(texto, textoMinusculas);
    ToLower(patron, patronMinusculas);
    return strstr(textoMinusculas, patronMinusculas) != NULL;
}

void GuardarProductos(Sproducto *lista){
	FILE *f = fopen("productos.txt", "w");
	if(!f) { 
		printf("Error al abrir el archivo .txt \n");
		return;
	}
	while(lista) {
		fprintf(f, "%d|%s|%s|%s\n",
			lista->codigo,
			lista->nombre,
			lista->marca,
			lista->descripcion);
		lista=lista->psig;
	}
	fclose(f);
}

void CargarProductos(Sproducto **lista) {
	FILE *f = fopen("productos.txt", "r");
	if(!f) {return;}
	while(1) {
		Sproducto *nuevo = new Sproducto;
		if (fscanf(f, "%d|%29[^|]|%29[^|]|%99[^\n]\n",
			&nuevo->codigo,
			nuevo->nombre,
			nuevo->marca,
			nuevo->descripcion) == 4) {
			nuevo->psig = *lista;
			*lista = nuevo;
		}
		else{
			delete nuevo;
			break;
		}
	}
	fclose(f);
}

void GuardarAsociados(Sasociado *lista){
	FILE *f = fopen("asociados.txt", "w");
	if (!f) {
	printf("error al abrir asociados.txt\n");
	return;
	}
	while (lista){
	fprintf(f,  "%d|%s|%s|%s\n",
		lista->codigo,
		lista->nombre,
		lista->direccion,
		lista->telefono);
	lista = lista->pnext;
	}
	fclose(f);
}

void CargarAsociados(Sasociado **lista) {
    FILE *f = fopen("asociados.txt", "r");
    if (!f) return;
    Sasociado *nuevo;
    while (1) {
        nuevo = new Sasociado;
        if (fscanf(f, "%d|%49[^|]|%99[^|]|%20[^\n]\n",
            &nuevo->codigo,
            nuevo->nombre,
            nuevo->direccion,
            &nuevo->telefono) == 4) {
            nuevo->pventas = NULL;
            nuevo->pnext = *lista;
            *lista = nuevo;
        } else {
            delete nuevo;
            break;
        }
    }
    fclose(f);
}

//============================ASOCIADOS============================
void AgregarAsociados(Sasociado **lista, Sasociado** asociado){
        (*asociado)->pnext=*lista;
        *lista=*asociado;
        printf("Codigo agregado con exito!");
}

Sasociado* NuevoAsociado(Sasociado *ListaAsociados){
    Sasociado *nuevo=new Sasociado;
    char nombre[100];
    char direccion[150];
    char telefono[21];
    int codigo;
	int valido=0;
    
    printf("Ingrese el codigo del Asociado: ");
    
    while (true) {
        int exito = scanf("%d", &codigo);
        if (exito != 1) {
            printf("El codigo es invalido, por favor intente de nuevo: ");
            while (getchar() != '\n');
            continue;
        }
        
        if (!BuscarCodigoAsociado(ListaAsociados, codigo)) {
            printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
            continue; 
        }
        break; 
    }

    while(!(BuscarCodigoAsociado(ListaAsociados, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }
    nuevo->codigo=codigo;

    printf("Ingrese el nombre del Asociado (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    while(strlen(nombre)>sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    }
    strcpy(nuevo->nombre, nombre);

    printf("Ingrese la direcccion del Asociado (No puede ser mayor de 100 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    while(strlen(direccion)>sizeof(nuevo->direccion)){
        printf("La direccion excede los 100 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    }
    strcpy(nuevo->direccion, direccion);

    printf("Ingrese el numero de telefono del Asociado: ");
    fflush(stdin);scanf(" %20[^\n]", telefono);fflush(stdin);
    while(strlen(telefono)>sizeof(nuevo->telefono)){
        printf("Se excede el numero de telefono.");
        fflush(stdin);scanf(" %20[^\n]", telefono);fflush(stdin);
    }
    while(!IsNumeric(telefono)){
        printf("El numero de telefono no es valido, por favor revise si hay una letra. ");
        fflush(stdin);scanf(" %20[^\n]", telefono);fflush(stdin);
    }
    strcpy(nuevo->telefono, telefono);

    nuevo->pnext=NULL;
    nuevo->pventas=NULL;
    return nuevo;
}

void MostrarAsociado(Sasociado *p){
    if(!p) return;
    printf("Nombre: %s | Direccion: %s | Numero de telefono: %s | Codigo del producto: %d\n", 
    p->nombre, p->direccion, p->telefono, p->codigo);
}

void ConsultarCodigoAsociado(Sasociado *lista, int n){
    if(!(lista)){
        printf("No hay asociados en la lista\n");
    }

    else{
        Sasociado *aux = lista;
        while(aux && aux->codigo != n){
            aux = aux->pnext;
        }

        if(aux && aux->codigo == n){
            MostrarAsociado(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

void BuscarPorNombreAsociado(Sasociado *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarAsociado(lista);
            encontrado = true;
        }
        lista = lista->pnext;
    }

    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

Sasociado* ExisteAsociado(Sasociado *lista, int n){
    Sasociado* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->pnext;
    }
    return ax;
}

void ModificarCodigoAsociado(Sasociado *nodoAModificar, Sasociado *cabezaLista){
    int CambioNum;
    int menu;
    char Cambio[150];

    if(nodoAModificar==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Direccion. \n");
    printf("3- Telefono. \n");
    printf("4- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin);
    switch(menu){
        case 1:
            printf("Ingrese el nombre: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(nodoAModificar->nombre)){
                printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(nodoAModificar->nombre, Cambio);
            fflush(stdin);
            printf("El nombre fue cambiado con exito!");
            printf("\n");
            system("pause");
            break;
        case 2:
            printf("Ingrese la direccion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(nodoAModificar->direccion)){
                printf("La direccion se excede los 100 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
            }
            strcpy(nodoAModificar->direccion, Cambio);
            fflush(stdin);
            printf("La direccion fue cambiada con exito!");
            printf("\n");
            system("pause");
            break;
        case 3:
            printf("Ingrese el numero de telefono del Asociado: ");
            fflush(stdin);scanf(" %20[^\n]", Cambio);fflush(stdin);
            while(strlen(Cambio)>sizeof(nodoAModificar->telefono)){
                printf("Se excede el numero de telefono.");
                fflush(stdin);scanf(" %20[^\n]", Cambio);fflush(stdin);
            }

            while(!IsNumeric(Cambio)){
                printf("El numero de telefono no es valido, por favor revise si hay una letra. ");
                fflush(stdin);scanf(" %20[^\n]", Cambio);fflush(stdin);
            }

            strcpy(nodoAModificar->telefono, Cambio);
            printf("Telefono cambiado con exito! \n");
            system("pause");
            break;
        case 4:
            printf("Ingrese el nuevo codigo: \n");
            while (true) {
                int exito = scanf("%d", &CambioNum);
                if (exito != 1) {
                    printf("El codigo es invalido, por favor intente de nuevo: ");
                    while (getchar() != '\n');
                    continue;
                }
                
                
                if (CambioNum == nodoAModificar->codigo) {
                    break; 
                }
        
                
                if (!BuscarCodigoAsociado(cabezaLista, CambioNum)) {
                    printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
                    continue; 
                }
                break; 
                }

            nodoAModificar->codigo=CambioNum;
            printf("\n");
            printf("Codigo cambiado con exito!");
            printf("\n");
            system("pause");
            break;
        default:
            printf("Por favor introduzca una opcion valida. \n");
            fflush(stdin);scanf("%d", &menu);fflush(stdin);
        }
    }
}

void EliminarPorCodigoAsociado(Sasociado **a, int n){
    Sasociado *aux;
    Sasociado *t;
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->pnext;
        delete aux;
    }
    else{
        aux=*a;
        while(aux->pnext && aux->pnext->codigo!=n){
            aux=aux->pnext;
        }
            if(aux->pnext!=NULL){
                t=aux->pnext;
                aux->pnext=t->pnext;
                delete t;
                printf("El codigo fue eliminado con exito!");
            }
            else{
                printf("El codigo no esta en la lista");
            }
    }
}

void MostrarAsociados(Sasociado *lista){
    while(lista){
        printf("Nombre: %s | Direccion: %s | Numero de telefono: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->direccion, lista->telefono, lista->codigo);
        lista=lista->pnext;
    }
}
//============================PRODUCTO============================
Sproducto* NuevoProducto(Sproducto *ListaProductos){ 
    Sproducto *nuevo=new Sproducto;
    char temp[100];
    char marca[100];
    char descripcion[150];
    int codigo;
	int entrada_valida;

    printf("Ingrese el codigo del producto: ");
    while (true) {
        int exito = scanf("%d", &codigo);
        if (exito != 1) {
            printf("El codigo es invalido, por favor intente de nuevo: ");
            while (getchar() != '\n');
            continue;
        }
        
        if (!BuscarCodigoProducto(ListaProductos, codigo)) {
            printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
            continue; 
        }
        break; 
        }

    printf("Ingrese el nombre (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", temp);  fflush(stdin); 
    while (strlen(temp)>=sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
        fflush(stdin);scanf(" %99[^\n]", temp);
    }
    strcpy(nuevo->nombre, temp);

    printf("Ingrese la marca (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);
    while(strlen(marca)>=sizeof(nuevo->marca)){
        printf("Muy largo. \n");
        fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);
    }
    strcpy(nuevo->marca, marca);

    printf("Ingrese el descripcion (No puede ser mayor a 100 caracteres): \n");
    fflush(stdin);scanf(" %99[^\n]", descripcion);fflush(stdin);
    while(strlen(descripcion)>=sizeof(nuevo->descripcion)){
        printf("Muy largo. \n");
        fflush(stdin);scanf(" %99[^\n]", descripcion);fflush(stdin);
    }
    strcpy(nuevo->descripcion, descripcion);

    nuevo->codigo=codigo;
    nuevo->psig=NULL;
    return nuevo;
    
}

void AgregarProductos(Sproducto **lista, Sproducto** producto){
        (*producto)->psig=*lista;
        *lista=*producto;
        printf("Codigo agregado con exito!");
}

void MostrarProductos(Sproducto *lista){
    while(lista){
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->marca, lista->descripcion, lista->codigo);
        lista=lista->psig;
    }
}

void MostrarProducto(Sproducto *p){
    if(!p) return;
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        p->nombre, p->marca, p->descripcion, p->codigo);
}

void ConsultarCodigo(Sproducto *lista, int n){
    if(!(lista)){
        printf("No hay productos en la lista\n");
    }

    else{
        Sproducto *aux = lista;
        while(aux && aux->codigo != n){
            aux = aux->psig;
        }

        if(aux && aux->codigo == n){
            MostrarProducto(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

void BuscarPorNombre(Sproducto *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarProducto(lista);
            encontrado = true;
        }
        lista = lista->psig;
    }

    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

Sproducto* ExisteProducto(Sproducto *lista, int n){
    Sproducto* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->psig;
    }
    return ax;
}

void ModificarCodigo(Sproducto *nodoAModificar, Sproducto *cabezaLista){
    int mod;
    int menu;
    char Cambio[150];

    if(nodoAModificar==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Marca. \n");
    printf("3- Descripcion. \n");
    printf("4- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin);
    switch(menu){
        case 1:
            printf("Ingrese el nombre: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(nodoAModificar->nombre)){
                printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(nodoAModificar->nombre, Cambio);
            fflush(stdin);
            printf("El nombre fue cambiado con exito!");
            printf("\n");
            system("pause");
            break;
        case 2:
            printf("Ingrese la marca: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(nodoAModificar->marca)){
                printf("La marca se excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
            }
            strcpy(nodoAModificar->marca, Cambio);
            fflush(stdin);
            printf("La marca fue cambiada con exito!");
            printf("\n");
            system("pause");
            break;
        case 3:
            printf("Ingrese la descripcion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(nodoAModificar->descripcion)){
                printf("La descripcion se excede los 100 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(nodoAModificar->descripcion, Cambio);
            fflush(stdin);
            printf("La descripcion fue cambiada con exito!");
            printf("\n");
            system("pause");
            break;
        case 4:
            printf("Introduzca el nuevo codigo: \n");
            while (true) {
                int exito = scanf("%d", &mod);
                if (exito != 1) {
                    printf("El codigo es invalido, por favor intente de nuevo: ");
                    while (getchar() != '\n');
                    continue;
                }
                
                
                if (mod == nodoAModificar->codigo) {
                    break;
                }
        
                
                if (!BuscarCodigoProducto(cabezaLista, mod)) {
                    printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
                    continue; 
                }
                break; 
                }
            nodoAModificar->codigo=mod;
            printf("\nCodigo cambiado con exito!\n");
            system("pause");
            break;
        default:
            printf("Por favor introduzca una opcion valida. \n");
            fflush(stdin);scanf("%d", &menu);fflush(stdin);
            }
        }
    }

void EliminarPorCodigo(Sproducto **a, int n){
    Sproducto *aux;
    Sproducto *t;
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->psig;
        printf("El codigo fue eliminado con exito!\n"); 
        delete aux;
    }
    else{
        aux=*a;
        while(aux->psig && aux->psig->codigo!=n){
            aux=aux->psig;
        }
            if(aux->psig!=NULL){
                t=aux->psig;
                aux->psig=t->psig;
                delete t;
                printf("El codigo fue eliminado con exito!");
            }
            else{
                printf("El codigo no esta en la lista");
            }
    }
}


//============================VENTAS============================


bool EsBisiesto(int anio){
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int DiasEnMes(int mes, int anio){
    int dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(mes == 2 && EsBisiesto(anio)) return 29;
    return dias[mes-1];
}


int PedirFecha(const char mensaje[]){
    int dia, mes, anio;
    printf("%s\n", mensaje);
    while(true){
        printf("  Dia (1-31): ");
        while(scanf("%d", &dia) != 1){
            printf("  Valor invalido, intente de nuevo: ");
            while(getchar() != '\n');
        }
        printf("  Mes (1-12): ");
        while(scanf("%d", &mes) != 1){
            printf("  Valor invalido, intente de nuevo: ");
            while(getchar() != '\n');
        }
        printf("  Anio (ej: 2026): ");
        while(scanf("%d", &anio) != 1){
            printf("  Valor invalido, intente de nuevo: ");
            while(getchar() != '\n');
        }
        while(getchar() != '\n');

        if(mes < 1 || mes > 12){
            printf("  Mes invalido, debe estar entre 1 y 12. Intente de nuevo.\n");
            continue;
        }
        if(anio < 1900 || anio > 2100){
            printf("  Anio invalido. Intente de nuevo.\n");
            continue;
        }
        if(dia < 1 || dia > DiasEnMes(mes, anio)){
            printf("  Ese dia no existe para ese mes/anio. Intente de nuevo.\n");
            continue;
        }
        break;
    }
    return anio * 10000 + mes * 100 + dia;
}

void FechaATexto(int fecha, char texto[11]){
    int anio = fecha / 10000;
    int mes  = (fecha / 100) % 100;
    int dia  = fecha % 100;
    sprintf(texto, "%02d/%02d/%04d", dia, mes, anio);
}


int siguienteNumOperacion = 1;


void InsertarVentaOrdenada(Sasociado *asociado, Sventa *nueva){
    if(!asociado->pventas || asociado->pventas->num_operacion > nueva->num_operacion){
        nueva->prventas = asociado->pventas;
        asociado->pventas = nueva;
        return;
    }
    Sventa *aux = asociado->pventas;
    while(aux->prventas && aux->prventas->num_operacion < nueva->num_operacion){
        aux = aux->prventas;
    }
    nueva->prventas = aux->prventas;
    aux->prventas = nueva;
}

void GuardarVentas(Sasociado *ListaAsociados){
    FILE *f = fopen("ventas.txt", "w");
    if(!f){
        printf("Error al abrir ventas.txt\n");
        return;
    }
    Sasociado *a = ListaAsociados;
    while(a){
        Sventa *v = a->pventas;
        while(v){
            fprintf(f, "%d|%d|%d|%d|%.2f|%.2f|%d\n",
                v->num_operacion,
                v->codigo_asociado,
                v->codigo_producto,
                v->cantidad,
                v->precio_unidad,
                v->monto_total,
                v->fecha);
            v = v->prventas;
        }
        a = a->pnext;
    }
    fclose(f);
}


void CargarVentas(Sasociado *ListaAsociados){
    FILE *f = fopen("ventas.txt", "r");
    if(!f) return;
    while(1){
        Sventa *nueva = new Sventa;
        int codigoAsoc;
        if(fscanf(f, "%d|%d|%d|%d|%f|%f|%d\n",
            &nueva->num_operacion,
            &codigoAsoc,
            &nueva->codigo_producto,
            &nueva->cantidad,
            &nueva->precio_unidad,
            &nueva->monto_total,
            &nueva->fecha) == 7){

            nueva->codigo_asociado = codigoAsoc;
            nueva->prventas = NULL;


            if(nueva->num_operacion >= siguienteNumOperacion){
                siguienteNumOperacion = nueva->num_operacion + 1;
            }

            Sasociado *a = ExisteAsociado(ListaAsociados, codigoAsoc);
            if(a){
                InsertarVentaOrdenada(a, nueva);
            } else {
                delete nueva;
            }
        } else {
            delete nueva;
            break;
        }
    }
    fclose(f);
}


void MostrarVenta(Sventa *v, Sasociado *ListaAsociados, Sproducto *ListaProductos){
    if(!v) return;
    Sasociado *a = ExisteAsociado(ListaAsociados, v->codigo_asociado);
    Sproducto *p = ExisteProducto(ListaProductos, v->codigo_producto);
    char fechaTxt[11];
    FechaATexto(v->fecha, fechaTxt);
    printf("N.Operacion: %d | Vendedor: %s | Producto: %s | Marca: %s | Cantidad: %d | Precio Unidad: %.2f | Monto Total: %.2f | Fecha: %s\n",
        v->num_operacion,
        a ? a->nombre : "(asociado no encontrado)",
        p ? p->nombre : "(producto no encontrado)",
        p ? p->marca  : "(marca no encontrada)",
        v->cantidad,
        v->precio_unidad,
        v->monto_total,
        fechaTxt);
}


Sventa* NuevaVenta(Sasociado *asociado, Sproducto *ListaProductos){
    int codigoProd;
    int cantidad;
    float precio;
    Sproducto *prodEncontrado = NULL;

    printf("Ingrese el codigo del producto: ");
    while(true){
        if(scanf("%d", &codigoProd) != 1){
            printf("Codigo invalido, por favor intente de nuevo: ");
            while(getchar() != '\n');
            continue;
        }
        prodEncontrado = ExisteProducto(ListaProductos, codigoProd);
        if(!prodEncontrado){
            printf("No existe un producto con ese codigo, intente de nuevo: ");
            continue;
        }
        break;
    }
    while(getchar() != '\n');

    printf("Producto seleccionado -> ");
    MostrarProducto(prodEncontrado);

    printf("Ingrese la cantidad vendida: ");
    while(true){
        if(scanf("%d", &cantidad) != 1 || cantidad <= 0){
            printf("Cantidad invalida (debe ser mayor a 0), intente de nuevo: ");
            while(getchar() != '\n');
            continue;
        }
        break;
    }
    while(getchar() != '\n');

    printf("Ingrese el precio por unidad: ");
    while(true){
        if(scanf("%f", &precio) != 1 || precio <= 0){
            printf("Precio invalido (debe ser mayor a 0), intente de nuevo: ");
            while(getchar() != '\n');
            continue;
        }
        break;
    }
    while(getchar() != '\n');

    Sventa *nueva = new Sventa;
    nueva->num_operacion = siguienteNumOperacion;
    siguienteNumOperacion++;
    nueva->codigo_producto = prodEncontrado->codigo;
    nueva->codigo_asociado = asociado->codigo;
    nueva->cantidad = cantidad;
    nueva->precio_unidad = precio;
    nueva->monto_total = cantidad * precio;
    nueva->fecha = PedirFecha("Ingrese la fecha de la venta:");
    nueva->prventas = NULL;

    return nueva;
}


void AgregarVenta(Sasociado *ListaAsociados, Sproducto *ListaProductos){
    if(!ListaAsociados){
        printf("No hay asociados registrados. Registre un asociado primero.\n");
        return;
    }
    if(!ListaProductos){
        printf("No hay productos registrados. Registre un producto primero.\n");
        return;
    }

    int codigoAsoc;
    Sasociado *asociado = NULL;

    printf("Ingrese el codigo del vendedor (asociado): ");
    while(true){
        if(scanf("%d", &codigoAsoc) != 1){
            printf("Codigo invalido, por favor intente de nuevo: ");
            while(getchar() != '\n');
            continue;
        }
        asociado = ExisteAsociado(ListaAsociados, codigoAsoc);
        if(!asociado){
            printf("No existe un asociado con ese codigo, intente de nuevo: ");
            continue;
        }
        break;
    }
    while(getchar() != '\n');

    printf("Vendedor seleccionado: %s\n\n", asociado->nombre);

    int seguir = 1;
    while(seguir){
        Sventa *nueva = NuevaVenta(asociado, ListaProductos);
        InsertarVentaOrdenada(asociado, nueva);
        GuardarVentas(ListaAsociados);

        printf("\nVenta registrada con exito:\n");
        MostrarVenta(nueva, ListaAsociados, ListaProductos);

        printf("\nDesea agregar otra venta para %s? (1 = Si, 0 = Volver al menu): ", asociado->nombre);
        while(scanf("%d", &seguir) != 1){
            printf("Opcion invalida, intente de nuevo: ");
            while(getchar() != '\n');
        }
        while(getchar() != '\n');
        printf("\n");
    }
}

// Busca una venta por su numero de operacion, recorriendo todos los
// asociados y, dentro de cada uno, todas sus ventas
Sventa* BuscarVentaPorOperacion(Sasociado *ListaAsociados, int numOperacion){
    Sasociado *a = ListaAsociados;
    while(a){
        Sventa *v = a->pventas;
        while(v){
            if(v->num_operacion == numOperacion) return v;
            v = v->prventas;
        }
        a = a->pnext;
    }
    return NULL;
}


void ConsultarVentaPorOperacion(Sasociado *ListaAsociados, Sproducto *ListaProductos){
    int numOperacion;

    printf("Ingrese el numero de operacion a consultar: ");
    while(scanf("%d", &numOperacion) != 1){
        printf("Numero invalido, por favor intente de nuevo: ");
        while(getchar() != '\n');
    }
    while(getchar() != '\n');

    Sventa *v = BuscarVentaPorOperacion(ListaAsociados, numOperacion);
    if(v){
        printf("\n");
        MostrarVenta(v, ListaAsociados, ListaProductos);
    } else {
        printf("No se encontro ninguna venta con el numero de operacion %d.\n", numOperacion);
    }
}


bool EliminarVentaPorOperacion(Sasociado *ListaAsociados, int numOperacion){
    Sasociado *a = ListaAsociados;
    while(a){
        Sventa *aux = a->pventas;
        Sventa *anterior = NULL;
        while(aux){
            if(aux->num_operacion == numOperacion){
                if(anterior == NULL){
                    a->pventas = aux->prventas;
                } else {
                    anterior->prventas = aux->prventas;
                }
                delete aux;
                return true;
            }
            anterior = aux;
            aux = aux->prventas;
        }
        a = a->pnext;
    }
    return false;
}


void EliminarVenta(Sasociado *ListaAsociados, Sproducto *ListaProductos){
    int numOperacion;

    printf("Ingrese el numero de operacion a eliminar: ");
    while(scanf("%d", &numOperacion) != 1){
        printf("Numero invalido, por favor intente de nuevo: ");
        while(getchar() != '\n');
    }
    while(getchar() != '\n');

    Sventa *v = BuscarVentaPorOperacion(ListaAsociados, numOperacion);
    if(!v){
        printf("No se encontro ninguna venta con el numero de operacion %d.\n", numOperacion);
        return;
    }

    printf("\nVenta encontrada:\n");
    MostrarVenta(v, ListaAsociados, ListaProductos);

    int confirmar;
    printf("\nEsta seguro que desea eliminar esta venta? (1 = Si, 0 = No): ");
    while(scanf("%d", &confirmar) != 1){
        printf("Opcion invalida, por favor intente de nuevo: ");
        while(getchar() != '\n');
    }
    while(getchar() != '\n');

    if(confirmar == 1){
        EliminarVentaPorOperacion(ListaAsociados, numOperacion);
        GuardarVentas(ListaAsociados);
        printf("\nLa venta fue eliminada con exito!\n");
    } else {
        printf("\nOperacion cancelada, la venta no fue eliminada.\n");
    }
}


void MostrarVentasEntreFechasPorVendedor(Sasociado *ListaAsociados, Sproducto *ListaProductos){
    if(!ListaAsociados){
        printf("No hay asociados registrados.\n");
        return;
    }

    int codigoAsoc;
    Sasociado *asociado = NULL;

    printf("Ingrese el codigo del vendedor: ");
    while(true){
        if(scanf("%d", &codigoAsoc) != 1){
            printf("Codigo invalido, por favor intente de nuevo: ");
            while(getchar() != '\n');
            continue;
        }
        asociado = ExisteAsociado(ListaAsociados, codigoAsoc);
        if(!asociado){
            printf("No existe un asociado con ese codigo, intente de nuevo: ");
            continue;
        }
        break;
    }
    while(getchar() != '\n');

    printf("Vendedor: %s\n\n", asociado->nombre);

    int fechaInicio = PedirFecha("Ingrese la fecha INICIAL del rango:");
    int fechaFin = PedirFecha("Ingrese la fecha FINAL del rango:");

    
    if(fechaInicio > fechaFin){
        int temp = fechaInicio;
        fechaInicio = fechaFin;
        fechaFin = temp;
    }

    printf("\n");
    bool encontrada = false;
    Sventa *v = asociado->pventas;
    while(v){
        if(v->fecha >= fechaInicio && v->fecha <= fechaFin){
            MostrarVenta(v, ListaAsociados, ListaProductos);
            encontrada = true;
        }
        v = v->prventas;
    }

    if(!encontrada){
        printf("No se encontraron ventas de %s en ese rango de fechas.\n", asociado->nombre);
    }
}

// ============================ REPORTES ============================

struct R41 {
    char marca[30];
    int cantidad;
    float monto_total;
};

struct R42 {
    char producto[30];
    int cantidad;
    float monto_total;
};

struct R43 {
    char producto[30];
    char marca[30];
    int cantidad;
    float monto_total;
};

struct R44 {
    char vendedor[50];
    char producto[30];
    char marca[30];
    int cantidad;
    float monto_total;
    int fecha;
};

void ReporteProducto(Sasociado *ListaAsociados, Sproducto *ListaProductos) {
    int codigoProd;
    printf("Ingrese el codigo del producto a consultar: ");
    while (scanf("%d", &codigoProd) != 1) {
        printf("Codigo invalido, intente de nuevo: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    Sproducto *prod = ExisteProducto(ListaProductos, codigoProd);
    if (!prod) { 
        printf("Producto no encontrado.\n"); 
        return; 
    }

    int count = 0;
    for (Sasociado *a = ListaAsociados; a != NULL; a = a->pnext) {
        for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
            if (v->codigo_producto == codigoProd) count++;
        }
    }
    if (count == 0) { 
        printf("No hay ventas registradas para este producto.\n"); 
        return; 
    }

    R41 *arr = new R41[count];
    int i = 0;
    for (Sasociado *a = ListaAsociados; a != NULL; a = a->pnext) {
        for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
            if (v->codigo_producto == codigoProd) {
                strcpy(arr[i].marca, prod->marca);
                arr[i].cantidad = v->cantidad;
                arr[i].monto_total = v->monto_total;
                i++;
            }
        }
    }

    
    for (int x = 0; x < count - 1; x++) {
        for (int y = x + 1; y < count; y++) {
            if (arr[x].cantidad < arr[y].cantidad) {
                R41 temp = arr[x];
                arr[x] = arr[y];
                arr[y] = temp;
            }
        }
    }

    printf("\n--- VENTAS DEL PRODUCTO: %s ---\n", prod->nombre);
    for (int x = 0; x < count; x++) {
        printf("Marca: %-15s | Cantidad: %-5d | Monto Total: %.2f\n", 
            arr[x].marca, arr[x].cantidad, arr[x].monto_total);
    }
    delete[] arr;
}

void ReporteMarca(Sasociado *ListaAsociados, Sproducto *ListaProductos) {
    char marca[30];
    printf("Ingrese la marca a consultar: ");
    scanf(" %29[^\n]", marca);
    while (getchar() != '\n');

    int count = 0;
    for (Sasociado *a = ListaAsociados; a != NULL; a = a->pnext) {
        for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
            Sproducto *p = ExisteProducto(ListaProductos, v->codigo_producto);
            if (p && ContainsIgnoreCase(p->marca, marca)) count++;
        }
    }
    if (count == 0) { 
        printf("No hay ventas registradas para esta marca.\n"); 
        return; 
    }

    R42 *arr = new R42[count];
    int i = 0;
    for (Sasociado *a = ListaAsociados; a != NULL; a = a->pnext) {
        for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
            Sproducto *p = ExisteProducto(ListaProductos, v->codigo_producto);
            if (p && ContainsIgnoreCase(p->marca, marca)) {
                strcpy(arr[i].producto, p->nombre);
                arr[i].cantidad = v->cantidad;
                arr[i].monto_total = v->monto_total;
                i++;
            }
        }
    }

    
    for (int x = 0; x < count - 1; x++) {
        for (int y = x + 1; y < count; y++) {
            if (arr[x].monto_total < arr[y].monto_total) {
                R42 temp = arr[x];
                arr[x] = arr[y];
                arr[y] = temp;
            }
        }
    }

    printf("\n--- VENTAS DE LA MARCA: %s ---\n", marca);
    for (int x = 0; x < count; x++) {
        printf("Producto: %-15s | Cantidad: %-5d | Monto Total: %.2f\n", 
            arr[x].producto, arr[x].cantidad, arr[x].monto_total);
    }
    delete[] arr;
}

void ReporteVendedor(Sasociado *ListaAsociados, Sproducto *ListaProductos) {
    int codigoAsoc;
    printf("Ingrese el codigo del vendedor: ");
    while (scanf("%d", &codigoAsoc) != 1) {
        printf("Codigo invalido, intente de nuevo: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    Sasociado *a = ExisteAsociado(ListaAsociados, codigoAsoc);
    if (!a) { 
        printf("Vendedor no encontrado.\n"); 
        return; 
    }

    int count = 0;
    for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
        count++;
    }
    if (count == 0) { 
        printf("No hay ventas registradas para este vendedor.\n"); 
        return; 
    }

    R43 *arr = new R43[count];
    int i = 0;
    for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
        Sproducto *p = ExisteProducto(ListaProductos, v->codigo_producto);
        if (p) {
            strcpy(arr[i].producto, p->nombre);
            strcpy(arr[i].marca, p->marca);
        } else {
            strcpy(arr[i].producto, "(Desconocido)");
            strcpy(arr[i].marca, "(Desconocida)");
        }
        arr[i].cantidad = v->cantidad;
        arr[i].monto_total = v->monto_total;
        i++;
    }

    
    for (int x = 0; x < count - 1; x++) {
        for (int y = x + 1; y < count; y++) {
            int cmp = strcmp(arr[x].producto, arr[y].producto);
            if (cmp > 0 || (cmp == 0 && strcmp(arr[x].marca, arr[y].marca) > 0)) {
                R43 temp = arr[x];
                arr[x] = arr[y];
                arr[y] = temp;
            }
        }
    }

    printf("\n--- VENTAS DEL VENDEDOR: %s ---\n", a->nombre);
    for (int x = 0; x < count; x++) {
        printf("Producto: %-15s | Marca: %-15s | Cantidad: %-5d | Monto Total: %.2f\n", 
            arr[x].producto, arr[x].marca, arr[x].cantidad, arr[x].monto_total);
    }
    delete[] arr;
}

void ReporteFechas(Sasociado *ListaAsociados, Sproducto *ListaProductos) {
    int fechaInicio = PedirFecha("Ingrese la fecha INICIAL del rango:");
    int fechaFin = PedirFecha("Ingrese la fecha FINAL del rango:");

    if (fechaInicio > fechaFin) {
        int temp = fechaInicio;
        fechaInicio = fechaFin;
        fechaFin = temp;
    }

    int count = 0;
    for (Sasociado *a = ListaAsociados; a != NULL; a = a->pnext) {
        for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
            if (v->fecha >= fechaInicio && v->fecha <= fechaFin) count++;
        }
    }
    if (count == 0) { 
        printf("No hay ventas registradas en ese rango de fechas.\n"); 
        return; 
    }

    R44 *arr = new R44[count];
    int i = 0;
    for (Sasociado *a = ListaAsociados; a != NULL; a = a->pnext) {
        for (Sventa *v = a->pventas; v != NULL; v = v->prventas) {
            if (v->fecha >= fechaInicio && v->fecha <= fechaFin) {
                strcpy(arr[i].vendedor, a->nombre);
                Sproducto *p = ExisteProducto(ListaProductos, v->codigo_producto);
                if (p) {
                    strcpy(arr[i].producto, p->nombre);
                    strcpy(arr[i].marca, p->marca);
                } else {
                    strcpy(arr[i].producto, "(Desconocido)");
                    strcpy(arr[i].marca, "(Desconocida)");
                }
                arr[i].cantidad = v->cantidad;
                arr[i].monto_total = v->monto_total;
                arr[i].fecha = v->fecha;
                i++;
            }
        }
    }


    for (int x = 0; x < count - 1; x++) {
        for (int y = x + 1; y < count; y++) {
            if (arr[x].fecha < arr[y].fecha) {
                R44 temp = arr[x];
                arr[x] = arr[y];
                arr[y] = temp;
            }
        }
    }

    printf("\n--- VENTAS ENTRE FECHAS ---\n");
    for (int x = 0; x < count; x++) {
        char fechaTxt[11];
        FechaATexto(arr[x].fecha, fechaTxt);
        printf("Fecha: %s | Vendedor: %-12s | Producto: %-12s | Marca: %-12s | Cantidad: %-4d | Monto: %.2f\n", 
            fechaTxt, arr[x].vendedor, arr[x].producto, arr[x].marca, arr[x].cantidad, arr[x].monto_total);
    }
    delete[] arr;
}

int main(){
    int menu=1;
    int option=1;
    int codigo;
    int CodigoAsociados;
    Sproducto* ListaProductos=NULL;
    Sasociado* ListaAsociados=NULL;
	Sproducto* prueba;
    Sasociado* pruebasociado;
	CargarProductos(&ListaProductos);
	CargarAsociados(&ListaAsociados);
	CargarVentas(ListaAsociados);
    while (menu!=0){
    system("cls");
    printf("\n\n\t\tSistema de ventas DirVen\n\n");  
    printf("1. Asociados \n");  
    printf("2. Productos \n"); 
    printf("3. Ventas \n");
    printf("4. Reportes \n");
    printf("\n0. Salir\n");  
    if (scanf("%d", &menu) != 1) {
            menu = -1; 
    }
    while (getchar() != '\n');

//======================================MENU ASOCIADOS======================================
    switch (menu){
        case 0: {
            system("cls");
            printf("Saliendo, hasta luego! \n");
            system("pause");
            return 0;
        }

        case 1: 
        while (option!=0){
        system("cls");
        printf("\n\n1.1.1.1 Agregar \n"); // CAMBIAR
        printf("1.1.1.2 Consultar por codigo \n");
        printf("1.1.1.3 Consultar por nombre \n");
        printf("1.1.1.4 Modificar por codigo \n");
        printf("1.1.1.5 Eliminar por codigo \n");
        printf("1.1.1.6 Mostrar todos los asociados \n");
        printf("1.1.1.0 Salir \n");
        fflush(stdin);
        if (scanf("%d", &option) != 1) {
            option = -1; 
        }
        while (getchar() != '\n');
            switch(option){
                case 0: {
                    break;
                }
                case 1: {
                    system("cls");
                    Sasociado* NewAsociado=NuevoAsociado(ListaAsociados);
                    AgregarAsociados(&ListaAsociados, &NewAsociado);
					GuardarAsociados(ListaAsociados);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 2: {
                    system("cls");
                    printf("Introduzca el codigo que desea consultar: ");
                    while (scanf("%d", &codigo) != 1) {
                        printf("El codigo es invalido, porfavor intente de nuevo. \n");
                        while (getchar() != '\n');
                        ("%d", &codigo);
                    }
                    ConsultarCodigoAsociado(ListaAsociados, codigo);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 3: {
                    char nombre[100];
                    system("cls");  
                    printf("Introduzca el nombre del asociado que desea consultar: \n");
                    fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    while(strlen(nombre)>sizeof(ListaAsociados->nombre)){
                        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
                        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    }
                    BuscarPorNombreAsociado(ListaAsociados, nombre);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 4: {
                    system("cls");  
                    printf("Que codigo deseas modificar: ");
                    while (scanf("%d", &codigo) != 1) {
                        printf("El codigo es invalido, porfavor intente de nuevo. \n");
                        scanf("%d", &codigo);
                        while (getchar() != '\n') {}; //Corregido el problema del buffer al insertar el codigo
                    }
                    pruebasociado=ExisteAsociado(ListaAsociados, codigo);
                    if(pruebasociado != NULL){
                        ModificarCodigoAsociado(pruebasociado, ListaAsociados);
                        GuardarAsociados(ListaAsociados);
                    } 
                    else {
                        printf("No se encontro el asociado con ese codigo.\n");
                        system("pause");
                    }
                    break;
                }
                case 5: {
                    system("cls");  
                    printf("Que codigo desea eliminar?: ");
                    while (scanf("%d", &codigo) != 1) {
                        printf("El codigo es invalido, porfavor intente de nuevo. \n");
                        while (getchar() != '\n');
                        ("%d", &codigo);
                    }
                    EliminarPorCodigoAsociado(&ListaAsociados, codigo);
					GuardarAsociados(ListaAsociados);
                    printf("El codigo fue eliminado con exito!");
                    printf("\n");
                    system("pause");
                    break;
                }
                case 6: {
                    system("cls");  
                    MostrarAsociados(ListaAsociados);
                    printf("\n");
                    system("pause");
                    break;
                }
                default: {
                    system("cls");
                    fflush(stdin);
                    printf("Por favor introduzca una opcion valida. \n");
                    printf("\n");
                    system("pause");
                }
            }
        }
        option=-1;
        break;
        
//======================================MENU PRODUCTOS======================================
        case 2: 
        while (option!=0){
        system("cls");
        printf("\n\n1.1.1.1 Agregar \n"); 
        printf("1.1.1.2 Consultar por codigo \n"); 
        printf("1.1.1.3 Consultar por nombre \n"); 
        printf("1.1.1.4 Modificar por codigo \n"); 
        printf("1.1.1.5 Eliminar por codigo \n");  
        printf("1.1.1.6 Mostrar todos los productos \n"); 
        printf("1.1.1.0 Salir \n");
        if (scanf("%d", &option) != 1) {
            option = -1; 
        }
        while (getchar() != '\n');
            switch(option){
                case 0:{
                    break;
                }
                case 1: {
                    system("cls");
                    Sproducto* NewProducto=NuevoProducto(ListaProductos);
                    AgregarProductos(&ListaProductos, &NewProducto);
					GuardarProductos(ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }

                case 2: {
                    system("cls");
                    printf("Introduzca el codigo que desea consultar: ");
                    while (scanf("%d", &codigo) != 1) {
                        printf("El codigo es invalido, porfavor intente de nuevo. \n");
                        while (getchar() != '\n');
                        ("%d", &codigo);
                    }
                    ConsultarCodigo(ListaProductos, codigo);
                    printf("\n");
                    system("pause");
                    break;
                }

                case 3: {
                    char nombre[100];
                    system("cls");
                    printf("Introduzca el nombre del producto que desea consultar: \n");
                    fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    while(strlen(nombre)>sizeof(ListaProductos->nombre)){
                        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
                        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    }
                    BuscarPorNombre(ListaProductos, nombre);
                    printf("\n");
                    system("pause");
                    break;
                }

                case 4: {
                    system("cls");
                    printf("Que codigo deseas modificar: ");
                    while (scanf("%d", &codigo) != 1) {
                        printf("El codigo es invalido, porfavor intente de nuevo. \n");
                        scanf(" %d", &codigo);
                        while (getchar() != '\n') {}; //Corregido el problema del buffer al insertar el codigo
                    }
                    prueba = ExisteProducto(ListaProductos, codigo);
                    if(prueba != NULL){
                        ModificarCodigo(prueba, ListaProductos);
                        GuardarProductos(ListaProductos);
                    } 
                    else {
                        printf("No se encontro el producto con ese codigo.\n");
                        system("pause");
                    }
                    break;
                }
                
                case 5: {
                    system("cls");
                    printf("Que codigo desea eliminar?: ");
                    while (scanf("%d", &codigo) != 1) {
                        printf("El codigo es invalido, porfavor intente de nuevo. \n");
                        while (getchar() != '\n');
                        ("%d", &codigo);
                    }
                    EliminarPorCodigo(&ListaProductos, codigo);
					GuardarProductos(ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 6: {
                    system("cls");
                    MostrarProductos(ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                default: {
                    system("cls");
                    printf("Por favor introduzca una opcion valida. ");
                    printf("\n");
                    system("pause");
                    break;
                }
            }
        }
        option=-1;
        break;

//======================================MENU VENTAS======================================
        case 3: {
            int opcionVentas = 1;
            while (opcionVentas != 0) {
                system("cls");
                printf("\n\n3.1 Agregar venta \n");
                printf("3.2 Consultar por numero de operacion \n");
                printf("3.3 Eliminar venta (por codigo de operacion) \n");
                printf("3.4 Mostrar todas las ventas entre dos fechas \n");
                printf("3.0 Salir \n");
                if (scanf("%d", &opcionVentas) != 1) {
                    opcionVentas = -1;
                }
                while (getchar() != '\n');

                switch (opcionVentas) {
                    case 0: {
                        break;
                    }
                    case 1: {
                        system("cls");
                        AgregarVenta(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    }
                    case 2: {
                        system("cls");
                        ConsultarVentaPorOperacion(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    }
                    case 3: {
                        system("cls");
                        EliminarVenta(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    }
                    case 4: {
                        system("cls");
                        MostrarVentasEntreFechasPorVendedor(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    }
                    default: {
                        system("cls");
                        printf("Por favor introduzca una opcion valida. \n");
                        system("pause");
                        break;
                    }
                }
            }
            option = -1;
            break;
        }

//======================================MENU REPORTES======================================
        case 4: {
            int opcionReportes = 1;
            while (opcionReportes != 0) {
                system("cls");
                printf("\n\n--- REPORTES ---\n");
                printf("4.1 Dado un producto mostrar todas sus ventas \n");
                printf("4.2 Dada una marca mostrar todas sus ventas \n");
                printf("4.3 Dado un vendedor mostrar todas sus ventas \n");
                printf("4.4 Mostrar todas las ventas entre dos fechas \n");
                printf("4.0 Salir \n");
                if (scanf("%d", &opcionReportes) != 1) {
                    opcionReportes = -1;
                }
                while (getchar() != '\n');

                switch (opcionReportes) {
                    case 0: break;
                    case 1:
                        system("cls");
                        ReporteProducto(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    case 2:
                        system("cls");
                        ReporteMarca(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    case 3:
                        system("cls");
                        ReporteVendedor(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    case 4:
                        system("cls");
                        ReporteFechas(ListaAsociados, ListaProductos);
                        printf("\n");
                        system("pause");
                        break;
                    default:
                        system("cls");
                        printf("Por favor introduzca una opcion valida.\n");
                        system("pause");
                        break;
                }
            }
            break;
        }
        default: {
            system("cls");
            printf("Por favor introduzca una opcion valida. \n");
            system("pause");
            break; 
        }
        }
    }
    return 0;
}
