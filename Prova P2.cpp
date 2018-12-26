/* Programa: ProvaFinal - P2
   Autor: Peterson Mashni
   Função: Permitir o cadastro de produtos e Pedidos
   Data: 06/12/2018
*/

// INCLUDES
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <iostream>


// ESTRUTURAS DE DADOS
typedef struct Produtos {
	int codigo;
	char nome[46];
	char unidade[3];
	char tipo[31];
} Produto;

typedef struct Pedidos {
	int numero;
	int codigoProduto;
	int quantidade;
	float valorUnitario;
	float valorTotal;
} Pedido;

// VARIAVEL UTILIZADA PARA CONTAGEM DO USO DAS OPCOES DO MENU
int contadorUso [3] = {0, 0, 0};

// DECLARACOES DE FUNÇÕES DE TELA E AMBIENTE
void cursor(int l, int c);
int getCursorCol();
int getCursorRow();

void say(int l, int c, char msg[]);
void say(int l, int c, int valor, char fmt[]);
void say(int l, int c, float valor, char fmt[]);

void getTexto(int l, int c, char* campo, int size);
void getInt(int l, int c, int* campo, int size);
void getFloat(int l, int c, float* campo, int size);

void fakeGet(int l, int c, char msg[], int size);
void fakeGet(int l, int c, int valor, char fmt[], int size);
void fakeGet(int l, int c, float valor, char fmt[], int size);

void window(int l1, int c1, int l2, int c2, char title[]);
void border(int l1, int c1, int l2, int c2);
void clear(int l1, int c1, int l2, int c2);
void limpaTela();

void message(char msg[]);
void message(char msg[], bool pause);

void alert(char msg[]);
void alert(char msg[], bool pause);

bool confirm(char mens[]);

WORD color(WORD cor);
void beep();

void initCharArray(char array[], char valor, int size);
void upperCharArray(char array[]);


// FUNÇÕES DE ACESSO A ARQUIVOS
bool buscaProduto(int codigo, Produto* produto);
bool salvaProduto(Produto produto);
int buscaUltimoNumeroPedido();
bool buscaPedido(int codigo, Pedido* pedido);
bool salvaPedido(Pedido pedido);

// CONSTANTES
const int borderTopLeft = 218;
const int borderHoriz = 196;
const int borderTopRight = 191;
const int borderVert = 179;
const int borderBottomRight = 217;
const int borderBottomLeft = 192;
const int borderJoinLeft = 195;
const int borderJoinRight = 180;

const WORD colorGet = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
const WORD colorBackground = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
const WORD colorTitle = 63;
const WORD colorMessage = 63;
const WORD colorWindow = 63;

// DECLARAÇÕES DE FUNÇÕES DE SERVIÇO
char menu();
void despedida();
float calculaTotalPedido(int quantidade, float valorUnitario);
void inclusaoProduto();
void consultaProduto();
void pedidoProduto();


// Função de chamada principal
int main ()
{
	setlocale(LC_ALL, "pt_BR.iso-8859-1");

	char op = ' ';
	do
	{	
		op = menu();
		switch(op)
		{
			case '1' : inclusaoProduto(); contadorUso[0]++; break;
			case '2' : consultaProduto(); contadorUso[1]++;  break;
			case '3' : pedidoProduto(); contadorUso[2]++;  break;
			case '9' : if (confirm("Confirma Saida")) op = 'x';
		}
	} while (op != 'x');
	
	despedida();
}

// monta o menu do sistema
char menu()
{
	color(colorBackground);
	system("cls");
	
	color(colorTitle);
	clear(1, 1, 3, 80);
	say(2, 0, "PROVA P2 - PEDIDO DE PRODUTO - LOGICA DA PROGRAMACAO - PROF JAIME GROSS");
	
	color(colorWindow);
	window(6, 5, 13, 35, "MENU");
	
	say(8,  7, "1 - Inclusao de Produtos");
	say(9,  7, "2 - Consulta de Produtos");
	say(10, 7, "3 - Pedido de Produtos");
	say(11, 7, "9 - Finalizar");
	
	char op = ' ';
	
	while( true )
	{	
		message("Selecione uma opcao: [ ]");
		cursor(24, 50);
		op = getch();
		printf("%c", op);
		
		if( (op < '1' || op > '3') & op != '9')
			  alert("Selecione uma opcao valida!", true);
		else
			return op;  
	}
}

// funcao executada antes de finalizar o programa que informa a contagem de uso de cadas opcao do menu
void despedida()
{
	system("cls");
	window(10, 10, 20, 70, "TERMINO DO PROGRAMA");
	say(12, 12, "Mapa de uso das opcoes do menu: ");
	
	say(14, 12, "Inclusao de Produto...:");
	say(15, 12, "Consulta de Produto...:");
	say(16, 12, "Pedido de Produtos....:");
	
	for(int i=0; i<3; i++)		
		fakeGet(14+i, 37, contadorUso[i], "%d", 3);
	
	message("Muito Obrigado! Press. qq tecla para finalizar!", true);
	system("cls");
}
	
// função de inclusao de Produtos
void inclusaoProduto()
{
	while(true)
	{
		int codigoProduto;
		Produto produto;
		
		produto.codigo = 0;
		initCharArray(produto.nome, ' ', 46);
		initCharArray(produto.unidade, ' ', 3);
		initCharArray(produto.tipo, ' ', 31);

		limpaTela();
		
		window(5, 4, 13, 72, "INCLUSAO DE PRODUTOS");
		message("Informe zero para retornar ao menu.");
		
		say(8,  6, "Codigo..:");
		say(10, 6, "Nome....:");
		say(11, 6, "Unidade.:");
		say(12, 6, "Tipo....:");
		
		
		getInt(8, 16, &codigoProduto, 5);
		
		if (codigoProduto == 0)
			return;
		else if (codigoProduto < 1)
			alert("Codigo invalido! Informe um codigo maior que zero!");
		else if(buscaProduto(codigoProduto, &produto))
		{
			fakeGet(10, 16, produto.nome, 50);
			fakeGet(11, 16, produto.unidade, 2);
			fakeGet(12, 16, produto.tipo, 30);
			message("Produto ja cadastrado! Press. qq tecla para continuar...", true);
		}
		else
		{
			produto.codigo = codigoProduto;
			getTexto(10, 16, produto.nome, 50);
			getTexto(11, 16, produto.unidade, 2);
			getTexto(12, 16, produto.tipo, 30);
			
			if (confirm("Salvar este Produto"))
				salvaProduto(produto);
		}
	}
}

// funcao de consulta de produtos
void consultaProduto()
{
	while(true)
	{
		int codigoProduto;
		Produto produto;
		
		limpaTela();
		
		window(5, 4, 13, 72, "CONSULTA DE PRODUTOS");
		message("Informe zero para retornar ao menu.");
		
		say(8,  6, "Codigo..:");
		say(10, 6, "Nome....:");
		say(11, 6, "Unidade.:");
		say(12, 6, "Tipo....:");
		
		if(buscaUltimoNumeroPedido() == 0)
		{
			alert("Nenhum produto cadastrado ainda, por favor, acesse a opcao [1] no menu!", true);
			return;
		}
		
		getInt(8, 16, &codigoProduto, 5);
		
		if (codigoProduto == 0)
			return;
		else if(buscaProduto(codigoProduto, &produto))
		{
			fakeGet(10, 16, produto.nome, 50);
			fakeGet(11, 16, produto.unidade, 2);
			fakeGet(12, 16, produto.tipo, 30);
			message("Press. qq tecla para continuar...", true);
		}
		else
			message("Produto nao Encontrado!", true);
	}	
}
			
void pedidoProduto()
{
	while(true)
	{
		int codigoProduto;
		
		Produto produto;
		Pedido pedido;
		
		pedido.numero = buscaUltimoNumeroPedido() + 1;
		pedido.codigoProduto = 0;
		pedido.quantidade = 0;
		pedido.valorUnitario = 0;
		pedido.valorTotal = 0;
	
		limpaTela();
		
		window(5, 4, 20, 77, "INCLUSAO DE PEDIDOS DE PRODUTOS");
		
		say(8,  6, "Numero Pedido....:");
		fakeGet(8, 25, pedido.numero, "%d", 5);
		
		say(10, 6, "Codigo Produto...:");
		say(11, 6, "Nome Produto.....:");
		
		say(13, 6, "Quantidade.......:");
		say(14, 6, "Valor Unitario...:");
		
		say(16, 6, "Valor Total......:");
				
		while(true)
		{
			message("Informe zero para retornar ao menu.");
			getInt(10, 25, &pedido.codigoProduto, 5);

			if(pedido.codigoProduto == 0)
				return;
			else if(buscaProduto(pedido.codigoProduto, &produto))
			{
				fakeGet(11, 25, produto.nome, 50);
				break;
			}
			else
				alert("Produto nao cadastrado!", true);
		}
		
		getInt(13, 25, &pedido.quantidade, 9);
		getFloat(14, 25, &pedido.valorUnitario, 12);

		pedido.valorTotal = calculaTotalPedido(pedido.quantidade, pedido.valorUnitario);
		fakeGet(16, 25, pedido.valorTotal, "%.2f", 12);
		
		if (confirm("Salvar este Pedido"))
			salvaPedido(pedido);
	}
}


float calculaTotalPedido(int quantidade, float valorUnitario)
{
	return quantidade * valorUnitario;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇÕES DE TRATAMENTO DE ARQUIVOS
/////////////////////////////////////////////////////////////////////////////////////////////

// Busca o registro de produto pelo codigo informado
bool buscaProduto(int codigo, Produto* produto)
{
	FILE *arqProduto; 
      
    arqProduto = fopen ("psm_produto.dat", "r"); 
    if (arqProduto == NULL) 
        return false;
      
    while(fread(produto, sizeof(Produto), 1, arqProduto)) 
    {
    	if(produto->codigo == codigo)
    	{
    		fclose(arqProduto);
    		return true;
    	}
    }
  
   	fclose(arqProduto);
   	return false;
} 

// salva o registro de produto em arquivo
bool salvaProduto(Produto produto)
{
	FILE *arqProduto;
	fpos_t pos;
	Produto _produto;
      
    arqProduto = fopen ("psm_produto.dat", "a+"); 
    if (arqProduto == NULL) 
        return false;
      
    pos = fgetpos(arqProduto, &pos);
    while(fread(&_produto, sizeof(Produto), 1, arqProduto)) 
    {
    	if(_produto.codigo == produto.codigo)
    	{
    		fsetpos(arqProduto, &pos);
    		fwrite(&produto, sizeof(Produto), 1, arqProduto);
    		fflush(arqProduto);
    		fclose(arqProduto);
    		return true;
    	}
  		pos = fgetpos(arqProduto, &pos);
  	}
  	
  	fwrite(&produto, sizeof(Produto), 1, arqProduto);
  	fflush(arqProduto);
   	fclose(arqProduto);
   	return false;
} 

// Retorno o Numero do Ultimo Pedido gerado, zero se nenhum
int buscaUltimoNumeroPedido()
{
	FILE *arqPedido; 
	Pedido pedido;
	int ultimoNumero = 0;
      
    arqPedido = fopen ("psm_pedido.dat", "r"); 
    if (arqPedido == NULL) 
        return 0;
      
    while(fread(&pedido, sizeof(Pedido), 1, arqPedido)) 
    	if(pedido.numero > ultimoNumero)
    		ultimoNumero = pedido.numero;
  
   	fclose(arqPedido);
   	return ultimoNumero;	
}

// Busca o registro de pedido pelo codigo informado
bool buscaPedido(int numero, Pedido* pedido)
{
	FILE *arqPedido; 
      
    arqPedido = fopen ("psm_pedido.dat", "r"); 
    if (arqPedido == NULL) 
        return false;
      
    while(fread(pedido, sizeof(Pedido), 1, arqPedido)) 
    {
    	if(pedido->numero == numero)
    	{
    		fclose(arqPedido);
    		return true;
    	}
    }
  
   	fclose(arqPedido);
   	return false;
} 

// salva o registro de pedido em arquivo
bool salvaPedido(Pedido pedido)
{
	FILE *arqPedido;
	fpos_t pos;
	Pedido _pedido;
      
    arqPedido = fopen ("psm_pedido.dat", "a+"); 
    if (arqPedido == NULL) 
        return false;
      
    pos = fgetpos(arqPedido, &pos);
    while(fread(&_pedido, sizeof(Pedido), 1, arqPedido)) 
    {
    	if(_pedido.numero == pedido.numero)
    	{
    		fsetpos(arqPedido, &pos);
    		fwrite(&pedido, sizeof(Pedido), 1, arqPedido);
    		fflush(arqPedido);
    		fclose(arqPedido);
    		return true;
    	}
  		pos = fgetpos(arqPedido, &pos);
  	}
  	
  	fwrite(&pedido, sizeof(Pedido), 1, arqPedido);
  	fflush(arqPedido);
   	fclose(arqPedido);
   	return false;
} 


/////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇÕES DE PRAPARAÇÃO DE TELA E AMBIENTE
/////////////////////////////////////////////////////////////////////////////////////////////

// prepara uma janela para trabalho
void window(int l1, int c1, int l2, int c2, char title[])
{
	WORD cor = color(FOREGROUND_BLUE);
	clear(l1+1, c1+1, l2+1, c2+1);
	color(cor);
	clear(l1, c1, l2, c2);
	border(l1, c1, l2, c2);
	cursor(l1, c1+2);
	printf("[ %s ]", title);
}

// posiciona o cursor na linha 'l' e coluna 'c'
void cursor(int l, int c)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){c-1,l-1});
}

// retorna coluna em que se encontra o cursor
int getCursorCol()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi);
    return cbsi.dwCursorPosition.X;
}

// retorna linha em que se encontra o cursor
int getCursorRow()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi);
    return cbsi.dwCursorPosition.Y;
}

// mostra uma mensagem nas coordenadas indicadas
void say(int l, int c, char msg[])
{
	if (c == 0)
	c = (80 - strlen(msg)) / 2;	

	cursor(l, c);
	printf(msg);
}

// mostra um int nas coordenadas indicadas, considerando o formato indicado (printf)
void say(int l, int c, int valor, char fmt[])
{
	cursor(l, c);
	printf(fmt, valor);
}

// mostra um float nas coordenadas indicadas, considerando o formato indicado (printf)
void say(int l, int c, float valor, char fmt[])
{
	cursor(l, c);
	printf(fmt, valor);
}

// simula um campo de entrada de dados com a informação fornecedida sem dar foco
void fakeGet(int l, int c, char msg[], int size)
{
	WORD _cor = color(colorGet);
	clear(l, c, l, c+size-1);
	cursor(l, c);
	printf(msg);
	color(_cor);
}

// simula um campo de entrada de dados do tipo int com a informação fornecedida sem dar foco
void fakeGet(int l, int c, int valor, char fmt[], int size)
{
	WORD _cor = color(colorGet);
	clear(l, c, l, c+size-1);
	cursor(l, c);
	printf(fmt, valor);
	color(_cor);
}

// simula um campo de entrada de dados do tipo float com a informação fornecedida sem dar foco
void fakeGet(int l, int c, float valor, char fmt[], int size)
{
	WORD _cor = color(colorGet);
	clear(l, c, l, c+size-1);
	cursor(l, c);
	printf(fmt, valor);
	color(_cor);
}

// realiza a entrada de uma informação texto
void getTexto(int l, int c, char* campo, int size)
{
	WORD cor = color(colorGet);
	clear(l, c, l, c+size-1);
	cursor(l,c);
	fgets(campo, size+1, stdin);
	fflush(stdin);
	color(cor);
}

// realiza a entrada de uma informação tipo int
void getInt(int l, int c, int* campo, int size)
{
	WORD cor = color(colorGet);
	clear(l, c, l, c+size-1);
	cursor(l,c);
	scanf("%d", campo);
	fflush(stdin);
	color(cor);
}

// realiza a entrada de uma informação tipo float
void getFloat(int l, int c, float* campo, int size)
{
	WORD cor = color(colorGet);
	clear(l, c, l, c+size-1);
	cursor(l,c);
	scanf("%f", campo);
	fflush(stdin);
	color(cor);
}

// monta uma borda nas coordenadas indicadas utilizando os caracteres informados
void border(int l1, int c1, int l2, int c2, int tl, int tr, int bl, int br)
{
	int i;
	say(l1, c1, tl, "%c");
	say(l1, c2, tr, "%c");
	
	say(l2, c1, bl, "%c");
	say(l2, c2, br, "%c");
	
	for(i=c1+1; i<c2; i++)
	{
		say(l1, i, borderHoriz, "%c");
		say(l2, i, borderHoriz, "%c");
	}
	   
	for(i=l1+1; i<l2; i++)
	{
		say(i, c1, borderVert, "%c");
		say(i, c2, borderVert, "%c");
	}
}

// monta uma borda simples nas coordenadas indicadas
void border(int l1, int c1, int l2, int c2)
{
	border(l1, c1, l2, c2, borderTopLeft, borderTopRight, borderBottomLeft, borderBottomRight);
}

// limpa determinada area da tela
void clear(int l1, int c1, int l2, int c2)
{
	int i, j;
		   
	for(i=l1; i<=l2; i++)
	{
		cursor(i, c1);
		
		for(j=c1; j<=c2; j++)
			printf(" ");
	}
}

// emite um beep
void beep()
{
	//printf("\a");
}

// gera mensagem na barra de mensagens com beep
void alert(char msg[])
{
	alert(msg, false);
}

// gera mensagem na barra de mensagens com beep, use true para realizar uma pausa
void alert(char msg[], bool pause)
{
	message(msg);
	beep();	
	if (pause)
		getch();
}

// gera mensagem na barra de mensagens
void message(char msg[])
{
	message(msg, false);
}

// gera mensagem na barra de mensagens, use true para realizar uma pausa
void message(char msg[], bool pause)
{
	clear(23, 2, 25, 79);
	say(24, (80-strlen(msg))/2, msg);
	if (pause)
	   getch();
}

// limpa a area de trabalho, mantendo as barras de titulo e mensagens
void limpaTela()
{
	WORD cor = color(colorBackground);
	clear(4, 1, 22, 80);
	color(cor);
}

// seta a cor informada, retornando a cor anterior
WORD color(WORD cor)
{
	HANDLE hstdin  = GetStdHandle( STD_INPUT_HANDLE  );
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );
	
	SetConsoleTextAttribute( hstdout, cor );
	return csbi.wAttributes;
}

// inicializa uma array de char, preenchendo com o valor informado
void initCharArray(char array[], char valor, int size)
{
	for (int i = 0; i < size; i++)
		array[i] = valor;
		
	array[size-1] = '\0';
}

// converte os caracteres de uma charArray para maiusculo
void upperCharArray(char array[])
{
	for (int i = 0; i < strlen(array); i++)
		if (isalpha(array[0]))
			array[i] = toupper(array[i]);
}

// solicita uma confirmação cuja resposta deva ser S/N, retornando um boolean referente a resposta (sim=true)
bool confirm(char mens[])
{
	char msgFinal [256];
	sprintf(msgFinal, "%s (S/N)? [ ]", mens);
		
	while(true)
	{		
		message(msgFinal);

		cursor(24, getCursorCol()-1);
		char op = getch();
		printf("%c", op);
		if ( op == 'S' | op == 's' | op == 'N' | op == 'n' )
			return op == 'S' | op == 's';
	} 
}
