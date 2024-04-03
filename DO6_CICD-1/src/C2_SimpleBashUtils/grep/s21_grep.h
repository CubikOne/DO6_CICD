#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_01 "s21_grep: %s: No such file or directory\n"

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int countFiles;    // счетчик файлов
  int numberLine;    // номер строки
  int countMatches;  // скок совпадений
} options;

void s21_grep_programm(int argc, char **argv);  // мейн функция для запуска
int scanOptions(int argc, char **argv, options *config,
                char **template);  // скан опций консоли
void printMainData(char *str, options *options, char *tmpl,
                   char *name);  // основные данные
void setConfigF(options *config, char **template, char *optarg);  // -F
void setConfigE(options *config, char **template, char *optarg);  // -E
void printfConfigO(regex_t my_regex, char *str, options config);  // -O
void s21_grep(char *name, options config, char *tmpl);  // греп в файле
void printfAuxData(options config, char *path);         // доп данные
void *increaseLengthStr(void *str, int size);  // увелечение длины строки
int createTemplate(char **str, char *optarg);  // шаблон
void setupConfig(options *config, int argc);  // наст. конфигирации
void addTemplate(char **str, char *optarg);  // добавление шаьлона

#endif