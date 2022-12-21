#include "demo.h"
#include "kernel.h"
#include "engine.h"
#include <string>

void demo::engine_demo(std::string str) {
	engine e;
	e.get_macro(str);
}
void demo::amazon_demo() {
	kernel bot;
	bot.move(870, 66);
	bot.click();
	bot.echo("amazon");
	bot.move(1057, 62);
	bot.click();
	bot.move(335, 408);
	bot.click();
	bot.move(445, 135);
	bot.click();
	bot.echo("black view");
	bot.move(526, 236);
	bot.click();
	bot.move(1060, 147);
	bot.click();
	bot.move(428, 648);
	bot.click();
	bot.move(1231, 656);
	bot.click();
}
void demo::wikipedia_demo() {
	kernel bot;
	bot.move(870, 66);
	bot.click();
	bot.echo("wikipedia");
	bot.move(1057, 62);
	bot.click();
	bot.move(231, 332);
	bot.click();
	bot.move(538, 604);
	bot.click();
	bot.echo("computer");
	bot.move(881, 614);
	bot.click();
	bot.move(18, 62);
	bot.click();
}
void demo::sendemail_demo() {
	kernel bot;
	bot.move(533, 743);
	bot.click();
	bot.move(870, 66);
	bot.click();
	bot.echo("yahoo");
	bot.move(1090, 65);
	bot.click();
	bot.move(218, 526);
	bot.click();
	bot.move(218, 424);
	bot.click();
	bot.move(75, 240);
	bot.click();
	bot.move(378, 243);
	bot.click();
	bot.echo("dr_lvr_mail@yahoo.it");
}