// ConsoleApplication11.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include "demo.h"
#include "uix.h"
#include "db_test.h"
#include "machine_view.h"

int main(){
	uix menu;
	menu.get_choice();
	db_test a;
	a.test_ascii();
	machine_view mv;
	mv.machine_view_test();
	mv.start_view();
}