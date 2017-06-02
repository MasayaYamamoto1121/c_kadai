#include <stdio.h>

#define TRUE  (1 == 1)
#define FALSE (1 != 1)

//入力バッファ用構造体、「num」=数字、「kind」=英字、「crlf」=改行コードを格納
typedef struct FILE_BUF
{
	char num[4];
	char kind;
	char crlf[2];
	
} FILE_BUF_TYPE;

//ファイルから1行読み込み構造体に格納する関数
int READ_ONE_LINE(FILE *fp, FILE_BUF_TYPE *data)
{
	int read = fscanf(fp, "%7c", data);
	return read;
}

//2種類のファイルから読み込んだ値を比較しフラグを立てる関数
int COMPARE(FILE_BUF_TYPE *dataA, FILE_BUF_TYPE *dataB)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		//Aファイルの数字がBファイルより小さければ-1、大きければ1を返す。
		if((dataA -> num[i]) < (dataB -> num[i]))
		{
			return -1;
		}
		else if((dataA -> num[i]) > (dataB -> num[i]))
		{
			return 1;
		}
	}
	//Aファイルの英字がBファイルより小さければ-1、大きければ1を返す。
	if((dataA -> kind) < (dataB -> kind))
	{
		return -1;
	}
	else if((dataA -> kind) < (dataB -> kind))
	{
		return 1;
	}
	return 0;
}

//構造体に格納した値をファイルに書き込む関数
void WRITE_ONE_LINE(FILE *fp, FILE_BUF_TYPE *data)
{
	fprintf(fp, "%c%c%c%c%c%c%c",
						data -> num[0],
						data -> num[1],
						data -> num[2],
						data -> num[3],
						data -> kind,
						data -> crlf[0],
						data -> crlf[1]);
}

//ファイルを開く関数
FILE *OPEN_FILE(char *filename, char *mode, int *status)
{
	FILE *fp = fopen(filename, mode);
	if(fp == NULL)
	{
		printf("%s：ファイルがオープン出来ません。\n",filename);
		*status = -1;
	}
	return fp;
}
		
int main(void)
{
	FILE *fpA, *fpB, *fpC;
	int status = 0;
	
	//ファイル別に構造体を定義し、領域を確保
	FILE_BUF_TYPE areaA;
	FILE_BUF_TYPE areaB;
	
	FILE_BUF_TYPE *dataA = &areaA;
	FILE_BUF_TYPE *dataB = &areaB;
	
	//条件、A、Bファイルに対応
	int looperA = TRUE;
	int looperB = TRUE;
	
	//ファイルの1行を読み込んだ時のフラグ変数
	int readA = TRUE;
	int readB = TRUE;
	
	//ファイルA、Bをリードモード、Cをライトモードで開く
	fpA = OPEN_FILE("./testA.txt", "r", &status);
	if(status != 0)
	{
		return status;
	}
	fpB = OPEN_FILE("./testB.txt", "r", &status);
	if(status != 0)
	{
		return status;
	}
	fpC = OPEN_FILE("./testC.txt", "w", &status);
	if(status != 0)
	{
		return status;
	}
	
	//ファイルの読み込みが出来なくなるまでのループ処理
	while(looperA || looperB)
	{
		//TRUEであれば1行進めての読み込み
		if(readA)
		{
			looperA = (EOF != READ_ONE_LINE(fpA, dataA));
		}
		if(readB)
		{
			looperB = (EOF != READ_ONE_LINE(fpB, dataB));
		}
		if(looperA && looperB)
		{
			//フラグの判定を行い、ファイルCに記述していく
			//ファイルに記述するとreadをTRUEにし、次の行に進む
			if(0 > COMPARE(dataA, dataB))
			{
				WRITE_ONE_LINE(fpC, dataA);
				
				readA = TRUE;
				readB = FALSE;
			}
			else if(0 < COMPARE(dataA, dataB))
			{
				WRITE_ONE_LINE(fpC, dataB);
				
				readA = FALSE;
				readB = TRUE;
			}
			else
			{
				WRITE_ONE_LINE(fpC, dataA);
				WRITE_ONE_LINE(fpC, dataB);
				
				readA = TRUE;
				readB = TRUE;
			}
		}
		else if(looperA)
		{
			WRITE_ONE_LINE(fpC, dataA);
			
			readA = TRUE;
			readB = FALSE;
		}
		else if(looperB)
		{
			WRITE_ONE_LINE(fpC, dataB);
			
			readA = FALSE;
			readB = TRUE;
		}
	}
	//開いたファイルは閉じる決まり
	fclose(fpC);
	fclose(fpB);
	fclose(fpA);
	
	return 0;
}