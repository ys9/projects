#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#define _XOPEN_SOURCE 600 //for powl() function


GdkPixbuf *create_pixbuf(const gchar *filename);
void print_result(void);
GtkWidget *resultLine;
char *expression;
int expLen = 0;
int num1Len = 0;
int num2Len = 0;
int curNum = 1;
bool operatorEnabled;
bool equalsSet;
bool num1DecimalSet;
bool num2DecimalSet;


int main(int argc, char *argv[])
{
	expression = calloc(50, sizeof(char));
	operatorEnabled = false;
	equalsSet = false;
	num1DecimalSet = false;
	num2DecimalSet = false;
    GtkBuilder *builder; 
    GtkWidget *window;
    GdkPixbuf *icon;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    resultLine = GTK_WIDGET(gtk_builder_get_object(builder, "resultLine"));
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_builder_connect_signals(builder, NULL);
    icon = create_pixbuf("calc.ico");
    gtk_window_set_icon(GTK_WINDOW(window), icon);
	g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}


GdkPixbuf *create_pixbuf(const gchar *filename)
{    
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if (!pixbuf) {
        fprintf(stderr, "%s\n", error -> message);
        g_error_free(error);
    }
    return pixbuf;
}


void on_window_main_destroy(void)
{
    gtk_main_quit();
}


void on_num0_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		if (num1Len != 0) {
			expression[expLen] = '0';
			expLen++;
			num1Len++;
			print_result();
		}
	}
	else if (curNum == 2) {
		if (num2Len != 0) {
			expression[expLen] = '0';
			expLen++;
			num2Len++;
			print_result();
		}
	}
}


void on_num1_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '1';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '1';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num2_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '2';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '2';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num3_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '3';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '3';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num4_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '4';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '4';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num5_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '5';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '5';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num6_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '6';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '6';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num7_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '7';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '7';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num8_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '8';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '8';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_num9_clicked(void)
{
	if (equalsSet == true) {
		gtk_label_set_text(GTK_LABEL(resultLine), "");
		equalsSet = false;
	}
	if (curNum == 1) {
		expression[expLen] = '9';
		expLen++;
		num1Len++;
		print_result();
	}
	else if (curNum == 2) {
		expression[expLen] = '9';
		expLen++;
		num2Len++;
		print_result();
	}
}


void on_add_clicked(void)
{
	if (expLen > 0 && equalsSet != true) {
		if (operatorEnabled == false) {
			expression[expLen] = ' ';
			expression[expLen + 1] = '+';
			expression[expLen + 2] = ' ';
			expLen += 3;
			operatorEnabled = true;
			curNum = 2;
			print_result();
		}
	}
}


void on_subtract_clicked(void)
{
	if (expLen > 0 && equalsSet != true) {
		if (operatorEnabled == false) {
			expression[expLen] = ' ';
			expression[expLen + 1] = '-';
			expression[expLen + 2] = ' ';
			expLen += 3;
			operatorEnabled = true;
			curNum = 2;
			print_result();
		}
	}
}


void on_multiply_clicked(void)
{
	if (expLen > 0 && equalsSet != true) {
		if (operatorEnabled == false) {
			expression[expLen] = ' ';
			expression[expLen + 1] = 'X';
			expression[expLen + 2] = ' ';
			expLen += 3;
			operatorEnabled = true;
			curNum = 2;
			print_result();
		}
	}
}


void on_divide_clicked(void)
{
	if (expLen > 0 && equalsSet != true) {
		if (operatorEnabled == false) {
			expression[expLen] = ' ';
			expression[expLen + 1] = '/';
			expression[expLen + 2] = ' ';
			expLen += 3;
			operatorEnabled = true;
			curNum = 2;
			print_result();
		}
	}
}


void on_exp_clicked(void)
{
	if (expLen > 0 && equalsSet != true) {
		if (operatorEnabled == false) {
			expression[expLen] = ' ';
			expression[expLen + 1] = '^';
			expression[expLen + 2] = ' ';
			expLen += 3;
			operatorEnabled = true;
			curNum = 2;
			print_result();
		}
	}
}


void on_decimal_clicked(void)
{
	if (curNum == 1 && num1DecimalSet == false) {
		expression[expLen] = '.';
		expLen++;
		num1Len++;
		num1DecimalSet = true;
		print_result();
	}
	else if (curNum == 2 && num2DecimalSet == false) {
		expression[expLen] = '.';
		expLen++;
		num2Len++;
		num2DecimalSet = true;
		print_result();
	}
}


void on_clear_clicked(void)
{
	expression = calloc(50, sizeof(char));
	expLen = 0;
	operatorEnabled = false;
	curNum = 1;
	num1Len = 0;
	num2Len = 0;
	equalsSet = false;
	num1DecimalSet = false;
	num2DecimalSet = false;
	gtk_label_set_text(GTK_LABEL(resultLine), "0");
}


void on_equals_clicked(void)
{
	char *leftExp = malloc(50);
	assert(leftExp);
	char *rightExp = malloc(50);
	assert(rightExp);
	int counter = 0;
	for (int i = 0; i < num1Len; i++)
		leftExp[i] = expression[i];
	for (int i = num1Len + 3; i <= expLen; i++) {
		rightExp[counter] = expression[i];
		counter++;
	}
	if (operatorEnabled == true && num2Len > 0 && num1DecimalSet == false && num2DecimalSet == false) {
		int lside = atoi(leftExp);
		int rside = atoi(rightExp);
		char out[1000] = {0};
		if (expression[num1Len + 1] == '+') {
			long sum = lside + rside;
			sprintf(out, "%ld", sum);
		}
		else if (expression[num1Len + 1] == '-') {
			long sum = lside - rside;
			sprintf(out, "%ld", sum);
		}
		else if (expression[num1Len + 1] == 'X') {
			long sum = lside * rside;
			sprintf(out, "%ld", sum);
		}
		else if (expression[num1Len + 1] == '^') {
			long sum = powl(lside, rside);
			sprintf(out, "%ld", sum);
		}
		else {
			double sum = (float)lside / (float)rside;
			sprintf(out, "%lf", sum);
			for (int i = 8; i >= 0; i--) { //truncate trailing zeros
				if (out[i - 1] == '.')
					break;
				else {
					if (out[i] == '0')
						out[i] = '\0';
				}
			}
		}
		gtk_label_set_text(GTK_LABEL(resultLine), out);
	}
	else if (operatorEnabled == true && num2Len > 0 && (num1DecimalSet == true || num2DecimalSet == true)) {
		double lside = atof(leftExp);
		double rside = atof(rightExp);
		char out[100] = {0};
		if (expression[num1Len + 1] == '+') {
			double sum = lside + rside;
			sprintf(out, "%lf", sum);
		}
		else if (expression[num1Len + 1] == '-') {
			double sum = lside - rside;
			sprintf(out, "%lf", sum);
		}
		else if (expression[num1Len + 1] == 'X') {
			double sum = lside * rside;
			sprintf(out, "%lf", sum);
		}
		else if (expression[num1Len + 1] == '^') {
			double sum = pow(lside, rside);
			sprintf(out, "%lf", sum);
		}
		else {
			double sum = lside / rside;
			sprintf(out, "%lf", sum);
		}
		for (int i = 8; i >= 0; i--) { //truncate trailing zeros
			if (out[i - 1] == '.')
				break;
			else {
				if (out[i] == '0')
					out[i] = '\0';
			}
		}
		gtk_label_set_text(GTK_LABEL(resultLine), out);
	}
	expression = calloc(50, sizeof(char));
	expLen = 0;
	operatorEnabled = false;
	curNum = 1;
	num1Len = 0;
	num2Len = 0;
	equalsSet = true;
	num1DecimalSet = false;
	num2DecimalSet = false;
	free(leftExp);
	free(rightExp);
}


void print_result(void)
{
	gtk_label_set_text(GTK_LABEL(resultLine), expression);
}