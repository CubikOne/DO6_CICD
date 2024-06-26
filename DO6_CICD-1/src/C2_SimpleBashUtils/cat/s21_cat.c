#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define GETOPT_OPTIONS_END -1  // конец списка опций
#define FOPEN_READ_MODE "r"    // для чтения файла
#define ASCII_DEL 127          // конст. для -v
#define NONPRINTING_SHIFT 64   // непечатаемые символы

static const char SHORT_OPTIONS[] = "AbeEnstTuv";  // короткие флаги

static const struct option LONG_OPTIONS[] = {
    {"show-all", 0, NULL, 'A'},
    {"number-nonblank", 0, NULL, 'b'},
    {"show-ends", 0, NULL, 'E'},
    {"number", 0, NULL, 'n'},
    {"squeeze-blank", 0, NULL, 's'},
    {"show-tabs", 0, NULL, 'T'},
    {"show-nonprinting", 0, NULL, 'v'},
    {"help", 0, NULL, 0},
    {NULL, 0, NULL, 0}};  // длинные флаги

typedef struct Options {  // структура для флагов
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool u;
  bool v;
} Options;
static void init_options(
    int argc, char* const argv[],
    Options* const
        options);  // инициализируем структуру оптионс по аргументам ком.строки
static void set_option(const char option,
                       Options* const options);  // флаг в структуре
static void print_help();                        // вывод хелпа
static void print_invalid_option();  //если ошибка при флаге

static void process_files(
    int file_count, char* const file_path[],
    const Options* const options);  // обработка файла: открытие чтение вывод
static void print_invalid_file(const char* const file_name);  //если нет файла
static void print_file(FILE* file, const Options* const options);  //вывод файла

static void squeeze_blank(
    FILE* file, const char previous_symbol, char current_symbol,
    const Options* const options);  //удаление строк при -s
static void number_line(const char previous_symbol, const char current_symbol,
                        const Options* const options);  // нумерация -n -b
static void end_line(const char current_symbol,
                     const Options* const options);  //доллар в конце -E
static void print_symbol(const char current_symbol,
                         const Options* const options);  //вывод символа
static void print_tab(const char current_symbol,
                      const Options* const options);  // символ табуляции -t
static void print_lfd(const char current_symbol);  // символ новой строки
static void print_cntrl(const char current_symbol,
                        const Options* const options);  // вывод управл.символов
static void print_meta(const char current_symbol,
                       const Options* const options);  //вывод метасимволов
static void print_plain(const char current_symbol);  // вывод симолов

int main(int argc, char* argv[]) {  // читаем аргументы строки флаг и файл
  Options options = {0};  // иницилизируем структуру
  init_options(argc, argv, &options);  // заполняем ее
  process_files(argc - optind, argv + optind, &options);
  return EXIT_SUCCESS;
}

static void init_options(int argc, char* const argv[], Options* const options) {
  int long_options_index = 0;  // получаем опцию
  char current_option =
      getopt_long(argc, argv, SHORT_OPTIONS, LONG_OPTIONS,
                  &long_options_index);  //обработка всех опций
  while (current_option != GETOPT_OPTIONS_END) {
    set_option(current_option, options);
    current_option = getopt_long(argc, argv, SHORT_OPTIONS, LONG_OPTIONS,
                                 &long_options_index);
  }
  if (options->b) {
    options->n = false;  // если -b, то сбрасываем -n
  }
}

static void set_option(
    const char option,
    Options* const options) {  //устанавливаем флаг в структуре
  switch (option) {
    case 'A':
      options->v = true;
      options->e = true;
      options->t = true;
      break;
    case 'b':
      options->b = true;
      break;
    case 'e':
      options->v = true;
      options->e = true;
      break;
    case 'E':
      options->e = true;
      break;
    case 'n':
      options->n = true;
      break;
    case 's':
      options->s = true;
      break;
    case 't':
      options->v = true;
      options->t = true;
      break;
    case 'T':
      options->t = true;
      break;
    case 'v':
      options->v = true;
      break;
    case 0:
      print_help();
      break;
    case '?':
    default:
      print_invalid_option();
  }
}

static void print_help() {
  fprintf(stderr,
          "Usage: cat [OPTION]... [FILE]...\n"
          "Concatenate FILE(s) to standard output.\n");
  exit(EXIT_FAILURE);
}

static void print_invalid_option() {
  fprintf(stderr, "Try 'cat --help' for more information.\n");
  exit(EXIT_FAILURE);
}

static void process_files(
    int file_count,
    char* const file_path[],  //обработка файла: открытие, чтение и вывод
    const Options* const options) {
  FILE* current_file = NULL;
  while (file_count > 0) {
    current_file = fopen(*file_path, FOPEN_READ_MODE);  // открытие файла
    current_file == NULL ? print_invalid_file(*file_path)
                         : print_file(current_file, options);
    fflush(stdout);  // очистка буфера
    ++file_path;
    --file_count;
  }
}

static void print_invalid_file(
    const char* const file_name) {  // если нельзя открыть файл
  fprintf(stderr, "cat: %s: No such file or directory\n", file_name);
}

static void print_file(
    FILE* file,
    const Options* const options) {  // чтение и вывод файла с флагами
  static char previous_symbol = '\n';
  char current_symbol = fgetc(file);
  while (!feof(file)) {
    number_line(previous_symbol, current_symbol, options);  // нумерация строк
    end_line(current_symbol, options);      // доллар в конце
    print_symbol(current_symbol, options);  //
    squeeze_blank(file, previous_symbol, current_symbol,
                  options);  // удаление пустых строк
    previous_symbol = current_symbol;
    current_symbol = fgetc(file);
  }
}

static void squeeze_blank(
    FILE* file, const char previous_symbol, char current_symbol,
    const Options* const options) {  // удаление строк при -s
  if (options->s && previous_symbol == '\n' && current_symbol == '\n') {
    current_symbol = fgetc(file);
    while (current_symbol == '\n') {
      current_symbol = fgetc(file);
    }
    ungetc(current_symbol, file);
  }
}

static void number_line(const char previous_symbol, const char current_symbol,
                        const Options* const options) {  //нумерация строк
  static unsigned line_count = 0;
  if (options->b && previous_symbol == '\n' && current_symbol != '\n') {
    fprintf(stdout, "%6u\t", ++line_count);
  } else if (options->n && previous_symbol == '\n') {
    fprintf(stdout, "%6u\t", ++line_count);
  }
}

static void end_line(const char current_symbol, const Options* const options) {
  if (options->e && current_symbol == '\n') {
    fputc('$', stdout);  // доллар в конце строки
  }
}

static void print_symbol(const char current_symbol,
                         const Options* const options) {
  if (isprint(current_symbol)) {
    print_plain(current_symbol);
  } else if (current_symbol == '\t') {
    print_tab(current_symbol, options);
  } else if (current_symbol == '\n') {
    print_lfd(current_symbol);
  } else if (iscntrl(current_symbol)) {
    print_cntrl(current_symbol, options);  // управл.символ
  } else {
    print_meta(current_symbol, options);  //мета символы
  }
}

static void print_tab(const char current_symbol, const Options* const options) {
  if (options->t) {
    print_plain('^');
    print_plain(current_symbol + NONPRINTING_SHIFT);
  } else {
    print_plain(current_symbol);  // замега табуляции на знак
  }
}

static void print_lfd(
    const char current_symbol) {  // вывод символа новой строки
  print_plain(current_symbol);
}

static void print_cntrl(const char current_symbol,
                        const Options* const options) {  // управляющие символы
  if (options->v) {
    print_plain('^');
    if (current_symbol == ASCII_DEL) {
      print_plain(current_symbol - NONPRINTING_SHIFT);
    } else {
      print_plain(current_symbol + NONPRINTING_SHIFT);
    }
  } else {
    print_plain(current_symbol);
  }
}

static void print_meta(const char current_symbol,
                       const Options* const options) {  // мета символы
  if (options->v) {
    const char meta_symbol = (signed char)current_symbol + ASCII_DEL + 1;
    print_plain('M');
    print_plain('-');
    if (isprint(meta_symbol)) {
      print_plain(meta_symbol);
    } else {
      print_cntrl(meta_symbol, options);
    }
  } else {
    print_plain(current_symbol);
  }
}

static void print_plain(const char current_symbol) {
  fputc(current_symbol, stdout);
}