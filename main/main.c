#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char term[50]; // Aranan kelime
int termFrequency = 0; // Bulunma sayısı
#define NUM_THREADS 5 // Thread sayısı

void *Thread_TermSearch(void *threadId)
{
	int tid = (int)threadId;

	char inputWord[50]; // Input dosyasındaki kelimeleri atmak için
	int i, count = 0;

	char filePath1[50] = "inputThread"; // Dosya yolu birleştirme
	filePath1[11] = (tid + 1) + '0';
	char filePath2[] = ".txt";
	strcat(filePath1, filePath2);

	FILE *inputFile;
	inputFile = fopen(filePath1, "r"); // Dosya açma

	while (fscanf(inputFile, "%s", inputWord) != EOF) // inputFile'dan okuduğu her kelimeyi inputWord array'ine atar
	{

		if (strcmp(inputWord, term) == 0) // Kelimeleri karşılaştır ve aynı ise sonucu bir artır
		{
			termFrequency++;
		}

		else if ((inputWord[0] == '(' || inputWord[0] == '"') && (inputWord[strlen(inputWord) - 1] == ')' || inputWord[strlen(inputWord) - 1] == '"') && strlen(term) + 2 == strlen(inputWord)) // Parantez ve tırnak işaretleri içindeki kelimeler
		{
			for (i = 0; i <= strlen(inputWord); i++)
			{
				if (inputWord[i + 1] == term[i]) // Harf bazında eşleştirme yapar
				{
					count++;
					if (count == strlen(term))
					{
						termFrequency++;
						count = 0;
					}
				}
				else
				{
					count = 0;
				}
			}
		}

		else if ((inputWord[0] == '"' || inputWord[0] == '(') && strlen(term) + 1 == strlen(inputWord)) // Tırnak işareti ve parantezle başlayan kelimeler
		{

			for (i = 0; i <= strlen(inputWord); i++)
			{
				if (inputWord[i + 1] == term[i])
				{
					count++;
					if (count == strlen(term))
					{
						termFrequency++;
						count = 0;
					}
				}
				else
				{
					count = 0;
				}
			}
		}

		else if ((inputWord[strlen(inputWord) - 1] == ',' || inputWord[strlen(inputWord) - 1] == '.' || inputWord[strlen(inputWord) - 1] == '"' || inputWord[strlen(inputWord) - 1] == ')') && strlen(term) + 1 == strlen(inputWord)) // Virgül, nokta, tırnak işareti ve parantez ile biten kelimeler
		{

			for (i = 0; i <= strlen(inputWord); i++)
			{
				if (inputWord[i] == term[i])
				{
					count++;
					if (count == strlen(term))
					{
						termFrequency++;
						count = 0;
					}
				}
				else
				{
					count = 0;
				}
			}
		}

		else if (inputWord[0] == '(' && inputWord[strlen(inputWord) - 2] == ')' && inputWord[strlen(inputWord) - 1] == ',' && strlen(term) + 3 == strlen(inputWord)) // Parantezden sonra virgül içeren kelimeler
		{

			for (i = 0; i <= strlen(inputWord); i++)
			{
				if (inputWord[i + 1] == term[i])
				{
					count++;
					if (count == strlen(term))
					{
						termFrequency++;
						count = 0;
					}
				}
				else
				{
					count = 0;
				}
			}
		}

	}

	fclose(inputFile); // Input dosyasını kapat
	pthread_exit(NULL); // Thread'leri kapatır

}

int main()
{
	scanf("%s", term);
	int errCheck;
	int i, j;
	pthread_t threads[NUM_THREADS]; // Thread array'i

	for (i = 0; i < NUM_THREADS; i++)
	{
		errCheck = pthread_create(&threads[i], NULL, Thread_TermSearch, (void *)i); // Thread oluşturur
		for (j = 0; j < NUM_THREADS; j++) // Çalışmayan thread'lerin çalışan thread'i beklemesini sağlar
			pthread_join(threads[j], NULL);
		if (errCheck) // Thread oluşturma kontrolü (0 ise hata yok, 1 ise thread oluşturulamadı)
		{
			printf("Thread not created");
			exit(-1);
		}
	}

	printf("Frequency: %d\n", termFrequency);
	pthread_exit(NULL);
}
