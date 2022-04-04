
#ifndef MENU_H_
#define MENU_H_

extern volatile int manual_temp, highlight, sensors[];
extern char terminal_messages[400];

void* execute_menu();

#endif /* MENU_H_ */