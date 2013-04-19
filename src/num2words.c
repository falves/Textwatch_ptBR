#include "num2words.h"

static const char* const ONES[] = {
  "",
  "e um",
  "e dois",
  "e três",
  "e quatro",
  "e cinco",
  "e seis",
  "e sete",
  "e oito",
  "e nove"
};

static const char* const TEENS[] ={
  "",
  "onze",
  "doze",
  "treze",
  "quatorze",
  "quinze",
  "dezesseis",
  "dezessete",
  "dezoito",
  "dezenove"
};

static const char* const TENS[] = {
  "",
  "dez",
  "vinte",
  "trinta",
  "quarenta",
  "cinquenta",
  "sessenta",
  "setenta",
  "oitenta",
  "noventa"
};

static const char* HORAS[] = {
  "",
  "uma",
  "duas",
  "três",
  "quatro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "onze"
};

static const char* STR_OH_CLOCK = "em ponto";
static const char* STR_MID = "e meia";


static size_t append_number(char* words, int num) 
{
  int tens_val = num / 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val == 1 && num != 10) 
  {
    strcat(words, TEENS[ones_val]);
    return strlen(TEENS[ones_val]);
  }

  strcat(words, TENS[tens_val]);
  len += strlen(TENS[tens_val]);

  if (tens_val < 1) 
  {
    strcat(words, ONES[ones_val]);
    return strlen(ONES[ones_val]);
  }

  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void fuzzy_minutes_to_words(PblTm *t, char* words) {
  int fuzzy_hours = t->tm_hour;
  int fuzzy_minutes = t->tm_min;

  size_t remaining = BUFFER_SIZE;
  memset(words, 0, BUFFER_SIZE);

  if (fuzzy_minutes != 0 || (fuzzy_hours != 12 && fuzzy_hours != 0)) 
  {
    if(fuzzy_minutes == 0)
    {
      remaining -= append_string(words, remaining, STR_OH_CLOCK);
    } 
    else if(fuzzy_minutes == 30)
    {     
      remaining -= append_string(words, remaining, STR_MID);
    } 
    else 
    {
      remaining -= append_number(words, fuzzy_minutes);
    }
  }
}

void fuzzy_sminutes_to_words(PblTm *t, char* words) {
  int fuzzy_minutes = t->tm_min;

  size_t remaining = BUFFER_SIZE;
  memset(words, 0, BUFFER_SIZE);

  if (fuzzy_minutes > 20)
  {
    remaining -= append_number(words,fuzzy_minutes%10);
  }
}

void fuzzy_hours_to_words(PblTm *t, char* words) {
  int fuzzy_hours = t->tm_hour;

  size_t remaining = BUFFER_SIZE;
  memset(words, 0, BUFFER_SIZE);

  // //Is it midnight?
  if (fuzzy_hours == 0) {
    remaining -= append_string(words, remaining, "meia-noite");
  //is it noon?
  } else if (fuzzy_hours == 12) {
    remaining -= append_string(words, remaining, "meio-dia");
  } else {
      remaining -= append_string(words, remaining, HORAS[fuzzy_hours%12]);
  }
}

void fuzzy_dates_to_words(PblTm *t, char* words) {
  string_format_time(words, BUFFER_SIZE, DATE_FORMAT, t);
}
