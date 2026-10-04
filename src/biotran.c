// Important libraries!!!
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Minimum amount of nucleotides required for a start and stop codon
// (disregarding mutations!)
#define MINNCLTD 6

// -Nucleotide translations-
// -------------------------

// DNA to mRNA
#define AD 'U'
#define TD 'A'

// mRNA to DNA
#define UM 'A'
#define AM 'T'

// General
#define G 'C'
#define C 'G'

// -------------------------

// Important Codons
#define MRNASTARTCODON "AUG"
#define DNASTARTCODON "ATG"
const char *MRNASTOPCODONS[] = {"UAG", "UAA", "UGA"};
const char *DNASTOPCODONS[] = {"TAG", "TAA", "TGA"};

// Commands required to run
typedef enum { dna, mrna, evaldna, evalmrna, CMDC } CMD;
const char *CMDSTRS[] = {"-dna", "-mrna", "-evaldna", "-evalmrna"};

// Function declarations
CMD getcmd(const char *arg);
char *dnatomrna(const char *fdna);
char *mrnatodna(const char *fmrna);
int eval_dna(const char *edna);
int eval_mrna(const char *emrna);

// Main
int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Error: No gene and/or translation type provided.\n\n");
    return 1;
  }
  if (strlen(argv[1]) < MINNCLTD) {
    printf("Error: Provided DNA/mRNA not long enough.\n\n");
    return 1;
  }

  for (int i = 0; argv[1][i] != '\0'; i++) {
    argv[1][i] = toupper((unsigned char)argv[1][i]);
  }

  CMD cmd = getcmd(argv[2]);

  switch (cmd) {
  case dna:
    char *resultmrna = dnatomrna(argv[1]);
    if (strlen(resultmrna) == strlen(argv[1])) {
      printf("DNA: %s\nmRNA: %s\n\n", argv[1], resultmrna);
    } else {
      printf("Error: Failed to translate provided DNA to mRNA.\n\n");
      return 1;
    }
    break;
  case mrna:
    char *resultdna = mrnatodna(argv[1]);
    if (strlen(resultdna) == strlen(argv[1])) {
      printf("mRNA: %s\nDNA: %s\n\n", argv[1], resultdna);
    } else {
      printf("Error: Failed to translate provided mRNA to DNA.\n\n");
      return 1;
    }
    break;
  case evaldna:
    int dresult = eval_dna(argv[1]);
    if (dresult) {
      printf("\nThe provided DNA is valid.\n\n");
    } else {
      printf("\nThe provided DNA is invalid.\n\n");
    }
    break;
  case evalmrna:
    int mresult = eval_mrna(argv[1]);
    if (mresult) {
      printf("\nThe provided mRNA is valid.\n\n");
    } else {
      printf("\nThe provided mRNA is invalid.\n\n");
    }
    break;
  default:
    printf("\nError: Invalid argument.\n\n");
    return 1;
  }

  return 0;
}

// Function definitions
CMD getcmd(const char *arg) {
  // Returns a cmd from CMD depending on arg
  for (int i = 0; i < CMDC; i++) {
    if (strcmp(CMDSTRS[i], arg) == 0) {
      return (CMD)i;
    }
  }
  return CMDC;
}

char *dnatomrna(const char *fdna) {
  // Translates DNA to mRNA
  char *resmrna = malloc(strlen(fdna) + 1);
  if (resmrna == NULL) {
    return NULL;
  }
  resmrna[0] = '\0';
  for (int i = 0; i < strlen(fdna); i++) {
    if (fdna[i] == UM) {
      resmrna[i] = AD;
    } else if (fdna[i] == AM) {
      resmrna[i] = TD;
    } else if (fdna[i] == G) {
      resmrna[i] = C;
    } else if (fdna[i] == C) {
      resmrna[i] = G;
    } else {
      resmrna[i] = '?';
    }
    resmrna[i + 1] = '\0';
  }
  return resmrna;
}

char *mrnatodna(const char *fmrna) {
  // Translates mRNA to DNA
  char *resdna = malloc(strlen(fmrna) + 1);
  if (resdna == NULL) {
    return NULL;
  }
  resdna[0] = '\0';
  for (int i = 0; i < strlen(fmrna); i++) {
    if (fmrna[i] == AD) {
      resdna[i] = UM;
    } else if (fmrna[i] == TD) {
      resdna[i] = AM;
    } else if (fmrna[i] == C) {
      resdna[i] = G;
    } else if (fmrna[i] == G) {
      resdna[i] = C;
    } else {
      resdna[i] = '?';
    }
    resdna[i + 1] = '\0';
  }
  return resdna;
}

int eval_dna(const char *edna) {
  // Evaluates whether or not edna (DNA) is valid DNA
  int length = strlen(edna);
  if (length % 3 != 0) {
    return 0;
  }
  if (edna[0] != DNASTARTCODON[0]) {
    return 0;
  } else if (edna[1] != DNASTARTCODON[1]) {
    return 0;
  } else if (edna[2] != DNASTARTCODON[2]) {
    return 0;
  }
  for (int i = 0; i < 3; i++) {
    if (edna[length - 3] == DNASTOPCODONS[i][0]) {
      if (edna[length - 2] == DNASTOPCODONS[i][1]) {
        if (edna[length - 1] == DNASTOPCODONS[i][2]) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int eval_mrna(const char *emrna) {
  // Evaluates whether or not emrna (mRNA) is valid mRNA
  int length = strlen(emrna);
  if (length % 3 != 0) {
    return 0;
  }
  if (emrna[0] != MRNASTARTCODON[0]) {
    return 0;
  } else if (emrna[1] != MRNASTARTCODON[1]) {
    return 0;
  } else if (emrna[2] != MRNASTARTCODON[2]) {
    return 0;
  }
  for (int i = 0; i < 3; i++) {
    if (emrna[length - 3] == MRNASTOPCODONS[i][0]) {
      if (emrna[length - 2] == MRNASTOPCODONS[i][1]) {
        if (emrna[length - 1] == MRNASTOPCODONS[i][2]) {
          return 1;
        }
      }
    }
  }
  return 0;
}
