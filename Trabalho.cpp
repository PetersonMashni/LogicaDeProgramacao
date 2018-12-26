/* Programa: CalculaVenda
   Autor: Peterson Mashni
   Função: Permitir o cadastro de clientes, veículos e vendas de veículos
   Data: 29/11/2018
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


// DECLARAÇÕES ESTRUTURAS DE DADOS
typedef struct Clientes {
	int codigo;
	char nome[51];
	float salario;
} Cliente;

typedef struct Veiculos {
	char placa[8];
	char marca[51];
	int ano;
	char tipoCombustivel[2];  // G-Gasolina, F-Flex
} Veiculo;

typedef struct Vendas {
	char placaVeiculo[8];
	int codigoCliente;
	float valorFabrica;
	char condicaoPagamento [2]; // V-A vista, P-A Prazo
	float valorEntrada;
	int numeroParcelas;
	float valorIPI;
	float lucroConcessionaria;
	float valorVenda;
	float valorParcela;
	float valorEncargos;
	float valorFinal;
} Venda;


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

// DECLARAÇÕES DE FUNÇÕES DE SERVIÇO
char menu();
void cliente();
void veiculo();
void venda();

void mostraResumoVista(Venda venda);
void mostraResumoPrazo(Venda venda);
void mostraVenda(Venda venda);

// FUNÇÕES DE ACESSO A ARQUIVOS
bool buscaCliente(int codigo, Cliente* cliente);
bool salvaCliente(Cliente cliente);

bool buscaVeiculo(char placa[8], Veiculo* veiculo);
bool salvaVeiculo(Veiculo veiculo);

bool buscaVenda(char placaVeiculo[8], Venda* venda);
bool salvaVenda(Venda venda);

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
			case '1' : cliente(); break;
			case '2' : veiculo(); break;
			case '3' : venda(); break;
			case '9' : if (confirm("Confirma Saida")) op = 'x';
		}
	} while (op != 'x');
	
	system("cls");
	printf("Programa finalizado. Ate Breve!");
	cursor(10, 0);
}

// monta o menu do sistema
char menu()
{
	color(colorBackground);
	system("cls");
	
	color(colorTitle);
	clear(1, 1, 3, 80);
	say(2, 0, "CALCULO VENDA VEICULO - LOGICA DA PROGRAMACAO - PROF JAIME GROSS");
	
	color(colorWindow);
	window(6, 5, 13, 31, "MENU");
	
	say(8,  7, "1 - Clientes");
	say(9,  7, "2 - Veiculos");
	say(10, 7, "3 - Calculo Valor Venda");
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

// função de cadastro de clientes
void cliente()
{
	while(true)
	{
		int codigoCliente;
		Cliente cliente;
		
		cliente.codigo = 0;
		initCharArray(cliente.nome, ' ', 51);
		cliente.salario = 0;
		
		limpaTela();
		
		window(5, 4, 13, 72, "CADASTRO DE CLIENTES");
		message("Informe zero para retornar ao menu.");
		
		say(8,  6, "Codigo..:");
		say(10, 6, "Nome....:");
		say(11, 6, "Salario.:");
		
		
		getInt(8, 16, &codigoCliente, 5);
		
		if (codigoCliente == 0)
			return;
		else if (codigoCliente < 1)
			alert("Codigo invalido! Informe um codigo maior que zero!");
		else if(buscaCliente(codigoCliente, &cliente))
		{
			fakeGet(10, 16, cliente.nome, 50);
			fakeGet(11, 16, cliente.salario, "%.2f", 12);
			message("Cliente ja cadastrado! Press. qq tecla para continuar...", true);
		}
		else
		{
			cliente.codigo = codigoCliente;
			getTexto(10, 16, cliente.nome, 50);
			getFloat(11, 16, &cliente.salario, 12);
			if (confirm("Salvar este Cliente"))
				salvaCliente(cliente);
		}
	}
}

// função auxiliar para exibir tipo de combustível
void mostraTipoCombustivel(int l, int c, char tipo)
{
	if (tipo == 'g' | tipo == 'G')
		fakeGet(l, c, "Gasolina", 20);
	else if (tipo == 'f' | tipo == 'F')
		fakeGet(l, c, "Flex", 20);
}

// função auxiliar para exibir condição de pagamento
void mostraCondicaoPagamento(int l, int c, char condicao)
{
	if (condicao == 'p' | condicao == 'P')
		fakeGet(l, c, "A Prazo", 7);
	else if (condicao == 'v' | condicao == 'V')
		fakeGet(l, c, "A Vista", 7);
}

// função de cadastro de veiculos
void veiculo()
{
	while(true)
	{
		char placaVeiculo [8];
		Veiculo veiculo;
		
		initCharArray(veiculo.placa, ' ', 8);
		initCharArray(veiculo.marca, ' ', 51);
		veiculo.ano = 0;
		initCharArray(veiculo.tipoCombustivel, ' ', 2);
		
		limpaTela();
		
		window(5, 4, 14, 77, "CADASTRO DE VEICULOS");
		message("Press. Enter para retornar ao menu.");
		
		say(8,  6, "Placa............:");
		say(10, 6, "Marca............:");
		say(11, 6, "Ano..............:");
		say(12, 6, "Tipo Combustivel.:");
		
		getTexto(8, 25, placaVeiculo, 7);
		upperCharArray(placaVeiculo);
			
		if (strcmp(placaVeiculo, "\n") == 0)
			return;
		else if(buscaVeiculo(placaVeiculo, &veiculo))
		{
			fakeGet(10, 25, veiculo.marca, 50);
			fakeGet(11, 25, veiculo.ano, "%d", 4);		
			mostraTipoCombustivel(12, 25, veiculo.tipoCombustivel[0]);
			
			message("Veiculo ja cadastrado! Press. qq tecla para continuar...", true);
		}
		else
		{
			strcpy(veiculo.placa, placaVeiculo);
			getTexto(10, 25, veiculo.marca, 50);
			getInt(11, 25, &veiculo.ano, 4);
			message("O Tipo de combustivel deve ser [G] Gasolina ou [F] Flex");
			while (true)
			{
				getTexto(12, 25, veiculo.tipoCombustivel, 1);
				if (veiculo.tipoCombustivel[0] != 'f'
					&veiculo.tipoCombustivel[0] != 'F'
					&veiculo.tipoCombustivel[0] != 'g'
					&veiculo.tipoCombustivel[0] != 'G')
					beep();
				else
					break;
			} 
			
			mostraTipoCombustivel(12, 25, veiculo.tipoCombustivel[0]);
			
			if (confirm("Salvar este Veiculo"))
				salvaVeiculo(veiculo);
		}
	}
}

// função auxiliar para exibir dados de uma venda já cadastrada
void mostraVenda(Venda venda)
{
	Veiculo veiculo;
	Cliente cliente;
	
	buscaVeiculo(venda.placaVeiculo, &veiculo);
	buscaCliente(venda.codigoCliente, &cliente);
	
	fakeGet(9, 25, veiculo.marca, 40);
	fakeGet(10, 25, veiculo.ano, "%4d", 4);
	fakeGet(10, 29, "/", 1);
	mostraTipoCombustivel(10, 30, veiculo.tipoCombustivel[0]);
	
	fakeGet(12, 25, venda.codigoCliente, "%d", 5);
	fakeGet(13, 25, cliente.nome, 40);
	fakeGet(14, 25, cliente.salario, "%.2f", 12);
	
	fakeGet(16, 25, venda.valorFabrica, "%.2f", 12);
	mostraCondicaoPagamento(16, 58, venda.condicaoPagamento[0]);		

	if(venda.condicaoPagamento[0] == 'v' | venda.condicaoPagamento[0] == 'V')
	{
		fakeGet(17, 25, "N/A", 12);
		fakeGet(18, 25, "N/A", 5);
		fakeGet(18, 58, "N/A", 12);			
	}
	else
	{
		fakeGet(17, 25, venda.valorEntrada, "%.2f", 12);
		fakeGet(18, 25, venda.numeroParcelas, "%d", 5);
		fakeGet(18, 58, venda.valorParcela, "%.2f", 12);			
	}

	message("Venda ja cadastrada! Pressione alguma tecla para ver o Resumo da Venda.", true);
	
	if(venda.condicaoPagamento[0] == 'v' | venda.condicaoPagamento[0] == 'V')
		mostraResumoVista(venda);
	else
		mostraResumoPrazo(venda);
}

// Função de cadastro de vendas
void venda()
{
	while(true)
	{
		char placaVeiculo [8];
		int codigoCliente;
		
		Veiculo veiculo;
		Cliente cliente;
		Venda venda;
		
		initCharArray(venda.placaVeiculo, ' ', 8);
		venda.codigoCliente = 0;
		venda.valorFabrica = 0;
		initCharArray(venda.condicaoPagamento, ' ', 2); // V-A vista, P-A Prazo
		venda.valorEntrada = 0;
		venda.numeroParcelas = 0;
		venda.valorIPI = 0;
		venda.lucroConcessionaria = 0;
		venda.valorVenda = 0;
		venda.valorParcela = 0;
		venda.valorEncargos = 0;
		venda.valorFinal = 0;
	
		limpaTela();
		
		window(5, 4, 20, 77, "VENDA DE VEICULOS");
		
		say(8,  6, "Placa Veiculo....:");
		say(9,  6, "Marca............:");
		say(10, 6, "Ano/Combustivel..:");
		say(12, 6, "Codigo Cliente...:");
		say(13, 6, "Nome.............:");
		say(14, 6, "Salario Cliente..:");
		say(16, 6, "R$ Fab. Veiculo..:");
		say(16, 39, "Condicao Pagto...:");
		say(17, 6, "R$ Entrada.......:");
		say(18, 6, "Num. de Parcelas.:");
		say(18, 39, "R$ das Parcelas..:");
		
		while(true)
		{
			message("Press. Enter para retornar ao menu.");
			getTexto(8, 25, placaVeiculo, 7);
			upperCharArray(placaVeiculo);
			
			if(buscaVenda(placaVeiculo, &venda))
			{
				mostraVenda(venda);
				break;
			}
			
			if (strcmp(placaVeiculo, "\n") == 0)
				return;
			else if(buscaVeiculo(placaVeiculo, &veiculo))
			{
				fakeGet(9, 25, veiculo.marca, 40);
				fakeGet(10, 25, veiculo.ano, "%4d", 4);
				fakeGet(10, 29, "/", 1);
				mostraTipoCombustivel(10, 30, veiculo.tipoCombustivel[0]);
				break;				
			}
			else
				alert("Veiculo nao cadastrado!", true);
		}
		
		// Retorna ao inicio se a venda já está cadastrada
		if(strcmp(venda.placaVeiculo, placaVeiculo) == 0)
			continue;
			
		while(true)
		{
			message("Informe o Cod do Cliente ou Zero para reiniciar.");
			getInt(12, 25, &codigoCliente, 5);
			
			if (codigoCliente == 0)
				break;
			else if(buscaCliente(codigoCliente, &cliente))
			{
				fakeGet(13, 25, cliente.nome, 40);
				fakeGet(14, 25, cliente.salario, "%.2f", 12);
				
				break;				
			}
			else
				alert("Cliente nao cadastrado!", true);
		}
		
		if (codigoCliente == 0)
			continue;
		
		strcpy(venda.placaVeiculo, placaVeiculo);
		venda.codigoCliente = codigoCliente;
		
		getFloat(16, 25, &venda.valorFabrica, 12);
		
		message("A condicao de pagamento deve ser [V] A Vista ou [P] A Prazo.");
		while (true)
		{
			getTexto(16, 58, venda.condicaoPagamento, 1);
			
			if (venda.condicaoPagamento[0] != 'v'
				&venda.condicaoPagamento[0] != 'V'
				&venda.condicaoPagamento[0] != 'p'
				&venda.condicaoPagamento[0] != 'P')
				beep();
			else
				break;
		} 
		message("");
	
		mostraCondicaoPagamento(16, 58, venda.condicaoPagamento[0]);
			
		// Calculos sobre valorFabrica
		venda.valorIPI = 
			(veiculo.tipoCombustivel[0] == 'f' | veiculo.tipoCombustivel[0] == 'F' ? 0.11 : 0.13) * venda.valorFabrica;
			
		venda.lucroConcessionaria = (venda.valorFabrica + venda.valorIPI) * 0.105;
		
		venda.valorVenda = venda.valorFabrica + venda.valorIPI + venda.lucroConcessionaria;
		
		venda.valorParcela = 0;
			
		if(venda.condicaoPagamento[0] == 'v' | venda.condicaoPagamento[0] == 'V')
		{
			fakeGet(17, 25, "N/A", 12);
			fakeGet(18, 25, "N/A", 5);
			fakeGet(18, 58, "N/A", 12);
			
			venda.valorEncargos = venda.valorIPI + venda.lucroConcessionaria;
			venda.valorFinal = venda.valorVenda;
		}
		else
		{
			getFloat(17, 25, &venda.valorEntrada, 12);
			while(true)
			{
				getInt(18, 25, &venda.numeroParcelas, 5);
				
				venda.valorParcela = 
					((venda.valorVenda - venda.valorEntrada) / venda.numeroParcelas) * pow(1.02, venda.numeroParcelas);
				
				fakeGet(18, 58, venda.valorParcela, "%.2f", 12);
				
				if(venda.numeroParcelas == 0)
					break;
				else if (venda.valorParcela > (cliente.salario * 0.3))
				{
					alert("Cliente nao tem renda suficiente para o Financiamento!", true);
					message("Tente aumentar o num. de parcelas ou zero para reiniciar.");
				}
				else
					break;
			}
			if (venda.numeroParcelas == 0)
				continue;
			
			venda.valorFinal = venda.valorEntrada + venda.numeroParcelas * venda.valorParcela;
			venda.valorEncargos = venda.valorFinal - venda.valorFabrica;
		}
		
		message("Pressione alguma tecla para ver o Resumo da Venda.", true);
		
		if(venda.condicaoPagamento[0] == 'v' | venda.condicaoPagamento[0] == 'V')
			mostraResumoVista(venda);
		else
			mostraResumoPrazo(venda);
		
		if (confirm("Salvar esta Venda"))
			salvaVenda(venda);
	}
}

// função auxiliar que mostra o resumo da venda a vista
void mostraResumoVista(Venda venda)
{
	window(8, 10, 14, 70, "Resumo da Venda a Vista");
	
	say(10, 12, "R$ Final do Veiculo:");
	say(11, 12, "R$ Fab. do Veiculo.:");
	say(12, 12, "R$ dos Encargos....:");
	
	fakeGet(10, 33, venda.valorFinal, "%.2f", 12);
	fakeGet(11, 33, venda.valorFabrica, "%.2f", 12);
	fakeGet(12, 33, venda.valorEncargos, "%.2f", 12);
	message("Pressione alguma tecla para continuar.", true);
}

// função auxiliar que mostra o resumo da venda a prazo
void mostraResumoPrazo(Venda venda)
{
	window(8, 10, 18, 70, "Resumo da Venda a Prazo");
	
	say(10, 12, "R$ Final do Veiculo:");
	say(11, 12, "R$ Fab. do Veiculo.:");
	say(12, 12, "R$ dos Encargos....:");
	
	say(14, 12, "R$ da Entrada......:");
	say(15, 12, "Num. de Parcelas...:");
	say(16, 12, "R$ das Parcela.....:");
	
	fakeGet(10, 33, venda.valorFinal, "%.2f", 12);
	fakeGet(11, 33, venda.valorFabrica, "%.2f", 12);
	fakeGet(12, 33, venda.valorEncargos, "%.2f", 12);
	
	fakeGet(14, 33, venda.valorEntrada, "%.2f", 12);
	fakeGet(15, 33, venda.numeroParcelas, "%d", 12);
	fakeGet(16, 33, venda.valorParcela, "%.2f", 12);
	
	message("Pressione alguma tecla para continuar.", true);
}


/////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇÕES DE TRATAMENTO DE ARQUIVOS
/////////////////////////////////////////////////////////////////////////////////////////////

// Busca o registro de cliente pelo codigo informado
bool buscaCliente(int codigo, Cliente* cliente)
{
	FILE *arqCliente; 
      
    arqCliente = fopen ("psm_cliente.dat", "r"); 
    if (arqCliente == NULL) 
        return false;
      
    while(fread(cliente, sizeof(Cliente), 1, arqCliente)) 
    {
    	if(cliente->codigo == codigo)
    	{
    		fclose(arqCliente);
    		return true;
    	}
    }
  
   	fclose(arqCliente);
   	return false;
} 

// Busca o registro de veiculo pela placa informada
bool buscaVeiculo(char placa[8], Veiculo* veiculo)
{
	FILE *arqVeiculo; 
    arqVeiculo = fopen ("psm_veiculo.dat", "r"); 
    
    if (arqVeiculo == NULL) 
        return false;
      
    while(fread(veiculo, sizeof(Veiculo), 1, arqVeiculo)) 
    {
    	if(strcmp(veiculo->placa, placa) == 0)
    	{
    		fclose(arqVeiculo);
    		return true;
    	}
    }
  
   	fclose(arqVeiculo);
   	return false;
} 

// Busca o registro de venda de veiculo pela placa informada
bool buscaVenda(char placaVeiculo[8], Venda* venda)
{
	FILE *arqVenda; 
      
    arqVenda = fopen ("psm_venda.dat", "r"); 
    if (arqVenda == NULL) 
        return false;
      
    while(fread(venda, sizeof(Venda), 1, arqVenda)) 
    {
    	if(strcmp(venda->placaVeiculo, placaVeiculo) == 0)
    	{
    		fclose(arqVenda);
    		return true;
    	}
    }
  
   	fclose(arqVenda);
   	return false;
} 

// salva o registro de venda em arquivo
bool salvaVenda(Venda venda)
{
	FILE *arqVenda;
	fpos_t pos;
	Venda _venda;
      
    arqVenda = fopen ("psm_venda.dat", "a+"); 
    if (arqVenda == NULL) 
        return false;
      
    pos = fgetpos(arqVenda, &pos);
    while(fread(&_venda, sizeof(Venda), 1, arqVenda)) 
    {
    	if(strcmp(_venda.placaVeiculo, venda.placaVeiculo) == 0)
    	{
    		fsetpos(arqVenda, &pos);
    		fwrite(&venda, sizeof(Venda), 1, arqVenda);
    		fflush(arqVenda);
    		fclose(arqVenda);
    		return true;
    	}
  		pos = fgetpos(arqVenda, &pos);
  	}
  	
  	fwrite(&venda, sizeof(Venda), 1, arqVenda);
  	fflush(arqVenda);
   	fclose(arqVenda);
   	return false;
} 

// salva o registro de cliente em arquivo
bool salvaCliente(Cliente cliente)
{
	FILE *arqCliente;
	fpos_t pos;
	Cliente _cliente;
      
    arqCliente = fopen ("psm_cliente.dat", "a+"); 
    if (arqCliente == NULL) 
        return false;
      
    pos = fgetpos(arqCliente, &pos);
    while(fread(&_cliente, sizeof(Cliente), 1, arqCliente)) 
    {
    	if(_cliente.codigo == cliente.codigo)
    	{
    		fsetpos(arqCliente, &pos);
    		fwrite(&cliente, sizeof(Cliente), 1, arqCliente);
    		fflush(arqCliente);
    		fclose(arqCliente);
    		return true;
    	}
  		pos = fgetpos(arqCliente, &pos);
  	}
  	
  	fwrite(&cliente, sizeof(Cliente), 1, arqCliente);
  	fflush(arqCliente);
   	fclose(arqCliente);
   	return false;
} 

// salva o registro de veiculo em arquivo
bool salvaVeiculo(Veiculo veiculo)
{
	FILE *arqVeiculo;
	fpos_t pos;
	Veiculo _veiculo;
      
    arqVeiculo = fopen ("psm_veiculo.dat", "a+"); 
    if (arqVeiculo == NULL) 
        return false;
      
    pos = fgetpos(arqVeiculo, &pos);
    while(fread(&_veiculo, sizeof(Veiculo), 1, arqVeiculo)) 
    {
    	if(strcmp(_veiculo.placa, veiculo.placa)==0)
    	{
    		fsetpos(arqVeiculo, &pos);
    		fwrite(&veiculo, sizeof(Veiculo), 1, arqVeiculo);
    		fflush(arqVeiculo);
    		fclose(arqVeiculo);
    		return true;
    	}
  		pos = fgetpos(arqVeiculo, &pos);
  	}
  	
  	fwrite(&veiculo, sizeof(Veiculo), 1, arqVeiculo);
  	fflush(arqVeiculo);
   	fclose(arqVeiculo);
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


