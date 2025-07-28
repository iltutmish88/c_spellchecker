#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define SIZE 100000
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"
#define PURPLE "\x1B[38;5;129m"
#define ORANGE "\x1B[38;5;208m"
#define RESET "\x1B[0m"
char dict[SIZE][40], in[1000][40];
int word_count1=0, word_count2=0;
typedef struct
{
    char target[40];
    int value;
}sort;

char* refine(char word[])
{
   int i=0, j=0;
   while (word[i])
   {
      if(isalpha(word[i]))
     {
       word[j]=tolower(word[i]);
       j++;
     }
     i++;
   }
   word[j]='\0';
   return word;

}

int levenshtein (char s1[], char s2[])
{
    int l1=strlen(s1), l2=strlen(s2);
    int dp[l1+1][l2+1];

    for (int i=0; i<l1+1; i++) dp[i][0]=i;
    for (int j=0; j<l2+1; j++) dp[0][j]=j;

    for (int i=1; i<l1+1; i++)
    {
        for (int j=1; j<l2+1; j++)
        {
            int cost=(s1[i-1]==s2[j-1])? 0:1;
            dp[i][j]=fmin(dp[i-1][j]+1, fmin(dp[i][j-1]+1, dp[i-1][j-1]+cost));
        }
    }
    return dp[l1][l2];
}

int soundex (char s1[], char s2[])
{
    int code[26]={0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0, 1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2}, i=1, j=1, score=0, t=1, w=1;
    char ph1[5], ph2[5];

    ph1[0]=s1[0], ph2[0]=s2[0];
    while (s1[i])
    {
        char c=s1[i];
        if (code[c-'a']!=0 && t<4)
        {
            ph1[t]=code[c-'a'];
            t++;
        }
        i++;
    }
    while (s2[j])
    {
        char c=s2[j];
        if (code[c-'a']!=0 && w<4)
        {
            ph2[w]=code[c-'a'];
            w++;
        }
        j++;
    }
    for (int k=1; k<5; k++)
    {
        if (ph1[k]==NULL) ph1[k]=0;
        if (ph2[k]==NULL) ph2[k]=0;
    }

    for (int k=0; k<4; k++)
    {
        if (ph1[k]!=ph2[k]) score+=1;
    }

    return score;
}

int appearance (int n)
{
    if (n<=1000) return 0;
    else if (n>1000 && n<=5000) return 1;
    else if (n>5000 && n<=10000) return 2;
    else if (n>10000 && n<=15000) return 3;
    else if (n>15000 && n<=20000) return 4;
    else if (n>20000 && n<=30000) return 5;
    else if (n>30000 && n<=40000) return 7;
    else if (n>40000 && n<=50000) return 8;
    else if (n>50000 && n<=70000) return 9;
    else if (n>80000 && n<=90000) return 10;
    else return 11;
}

int comparison(const void *a, const void *b)
{
    const sort *recordA = (const sort *)a;
    const sort *recordB = (const sort *)b;
    return (recordA->value - recordB->value);
}

int main()
{
  FILE *dictionary, *input;
  int found1=0, found2=0, f=0, t=0, x, y, score;
  char temp1[40], temp2[40];
  float p;

  dictionary=fopen("dictionary.txt","r");
  if (dictionary==NULL)
   {
     printf("Error opening file!!!");
     return 1;
   }
   while (word_count1<SIZE && fscanf(dictionary, "%s", dict[word_count1])==1)
   {
       word_count1++;
   }
   fclose(dictionary);

   input=fopen("input.txt","r");
   if (input==NULL)
   {
     printf("Error opening file!!!");
     return 1;
   }
   while (word_count2<1000 && fscanf(input, "%s", in[word_count2])==1)
   {
       word_count2++;
   }
   fclose(input);

   printf(PURPLE "  ____  ____  ____                           ____       ____ ___        ______      \n");
   printf(       " |    ||    ||     |    |      |       /\\   |    | \\  /|    | |  |\\   |   |  |    |\n");
   printf(       " |____ |____||____ |    |   __ |      /__\\  |____|  \\/ |____| |  | \\  |   |  |____|\n");
   printf(       "      ||     |     |    |      |     /    \\ |    |  |  |    | |  |  \\ |   |  |    |\n");
   printf(       " |____||     |____ |____|____  |____/      \\|____|  |  |    |_|_ |   \\|   |  |    |\n\n" RESET);
   printf(BLUE "                     --||" GREEN  "      CODE.    CHECK.    CORRECT.      " BLUE "||--\n");
   printf(ORANGE "                       --||" GREEN "  SPELLCHECKER v2.0 - BY GROUP-2  " ORANGE"||--\n\n" RESET);

   printf("The misspelled words are shown in ");
   printf(RED "red colour:\n" RESET);
   for (int i=0; i<word_count2; i++)
   {
      strcpy(temp1, in[i]);
      refine(temp1);
      for (int j=0; j<word_count1; j++)
      {
          x=0;
          strcpy(temp2, dict[j]);
          refine(temp2);
          if(strcmp(temp1, temp2)==0)
          {
            printf("%s ", in[i]);
            x=1;
            break;
          }
      }
          if (x==0)
          {
            printf(RED "%s " RESET, in[i]);
            t++;
          }
   }
   printf("\n\n\n");

   printf(PURPLE "%-20s |%15s | %40s\n" RESET, "Misspelled-Word", "Word-Position", "Possible-Correct-Spellings");
   printf(PURPLE "----------------------------------------------------------------------------------------------------\n" RESET);

   for (int i=0; i<word_count2; i++)
   {
      found1=0, found2=0, f=0, y=0;
      strcpy(temp1, in[i]);
      refine(temp1);
      sort srt[100];
      for (int j=0; j<word_count1; j++)
      {
          strcpy(temp2, dict[j]);
          refine(temp2);
          if(strcmp(temp1, temp2)==0)
          {
              found1=1;
              break;
          }
      }
      if (!found1)
      {
          for (int k=0; k<i; k++)
          {
              if(strcmp(temp1, refine(in[k]))==0)
              {
                 found2=1;
                 break;
              }
          }
      }

      if (!found1 && !found2)
      {
        printf(ORANGE "%-20s " PURPLE "|" BLUE "%-15d" PURPLE "|", temp1, i+1);

        for (int g=0; g<word_count1; g++)
        {
            strcpy(temp2, dict[g]);
            refine(temp2);
            score=levenshtein(temp1, temp2) + soundex(temp1, temp2) + appearance(g+1);
            if (score<5)
            {
                strcpy(srt[y].target, dict[g]);
                srt[y].value=score;
                y++;
                if (y>=100) break;
            }
        }

        qsort(srt, y, sizeof(sort), comparison);
        for (int q=0; q<5 && q<y; q++)
        {
           printf(GREEN "%s\t" RESET, srt[q].target);
        }
        printf("\n");
      }
   }

    p=(float)(word_count2-t)*100.0/word_count2;
    printf(BLUE "\nTotal Words: %d \n" RESET, word_count2);
    printf(BLUE "Misspelled words: %d\n" RESET, t);
    printf(BLUE "Accuracy: %.2f%% \n " RESET, p);
    return 0;
}
