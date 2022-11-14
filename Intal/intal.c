#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void *intal_create(const char *str)
{
  int len = strlen(str), i=0;
  char *K = NULL;
  void *Intal = NULL;
  if(len==1)
  {
    char *m = (char*)malloc(2*sizeof(char));
    m[0]=str[0];
    m[1]='\0';
    Intal = m;
    return Intal;
  }
  int w=0;
  while(str[w]=='0')
  {
    w++;
  }
  int len1=len;
  len=len-w;
  i=w;
  K = (char*)calloc(len+1,sizeof(char));
  while(i<len1)
  {
    if((str[i-w]<48) || (str[i-w]>57))
    {
      if(i-w==0)
      {
        free(K);
        return NULL;
      }
      else
      {
        K[i]='\0';
        Intal=K;
        break;
      }
    }
    else
    {
      K[i-w]=str[i];
    }
    i++;
  }
  K[i-w]='\0';
/*  while((K[0]=='0')&&(len>1))
  {
    K=K+1;
    len--;
  }
*/
  Intal = K;
  return Intal;
}

char* intal2str(void *intal)
{
  if(intal==NULL)
  {
    return NULL;
  }
  char *X = (char*)intal;
  int len = strlen(X), i=0;
  if((len==1)&&(X[0]=='0'))
  {
    char *m = (char*)calloc(2, sizeof(char));
    m[0]='0';
    m[1]='\0';
    return m;

  }
  char *K = NULL;
  int j=0;
  while((X[i]=='0')&&(j<=len+1))
  {
    i++;
    j--;
  }
  K = (char*)calloc(len+1-j, sizeof(char));
  for(i=0; i<len-j; i++)
  {
    K[i]=X[i+j];
  }
  K[len]='\0';
  return K;
}

void intal_destroy(void *intal)
{
  if(intal!=NULL)
  {
    free(intal);
  }
}

char Inc_Dig(char x)
{
  if(x!='9')
  {
    return (x+1);
  }
  else
  {
    return '0';
  }
}
void* intal_increment(void *intal)
{
  if(intal==NULL)
  {
    return intal_create("0");
  }
  char *str = intal2str(intal);
  int len = strlen(str);
  int p = len+1;
  int i;
  char *Res = NULL;
  Res = (char*)calloc(len+2, sizeof(char));
  Res[0]='0';
  for(i=len-1; i>=0; i--)
  {
    Res[i+1]=str[i];

  }
  Res[p]='\0';
  Res[p-1]=Inc_Dig(Res[p-1]);
  i=0;
  while(i<len)
  {
    if((Res[p-i-1]=='0')&&(str[len-i-1]=='9'))
    {
      Res[p-i-2]=Inc_Dig(Res[p-i-2]);
    }
    else
    {
      break;
    }
    i++;
  }
  free(str);
  void *Intal = intal_create(Res);
  free(Res);
  return Intal;
}

char Dec_Dig(char x)
{
  if(x!='0')
  {
    return x-1;
  }
  else
  {
    return '9';
  }
}

int intal_compare(void* intal1, void* intal2)
{
	if((intal1==NULL)||(intal2==NULL))
	{
		return -2;
	}
	char *str1 = intal2str(intal1);
	char *str2 = intal2str(intal2);
  int len1=strlen(str1);
	int len2=strlen(str2);
  if(len1 > len2)
	{
    free(str1);
    free(str2);
		return 1;
	}

	if(len2 > len1)
	{
    free(str1);
    free(str2);
		return -1;
	}
	else
	{
		int i=0;
		while(i<len1)
		{
			if(str1[i]>str2[i])
			{
        free(str1);
        free(str2);
				return 1;
			}
			if(str2[i]>str1[i])
			{
        free(str1);
        free(str2);
				return -1;
			}
			i++;
		}
    free(str1);
    free(str2);
		return 0;
	}
}

void *intal_decrement(void *intal)
{
  if(intal==NULL)
  {
    return intal_create("0");
  }

  void *Z = intal_create("0");
  if(intal_compare(intal, Z)==0)
  {
    return Z;
  }
  char *str = intal2str(intal);
  int len = strlen(str), i=0;
  if((len==1)&&(str[0]=='0'))
  {
      free(str);
      return intal_create("0");
  }
  char *Res = (char*)calloc(len+1, sizeof(char));
  for(i=len-1; i>-1; i--)
	{
	    Res[i]=str[i];
	}
  Res[len]='\0';
  Res[len-1]=Dec_Dig(Res[len-1]);
  i=0;
  while(i<len)
  {
    if((Res[len-i-1]=='9')&&(str[len-i-1]=='0'))
    {
      Res[len-i-2]=Dec_Dig(Res[len-i-2]);
    }
    else
    {
      break;
    }
    i++;
  }
  intal_destroy(Z);
  free(str);
  void *Intal= intal_create(Res);
  free(Res);
  return Intal;
}

int con_int(char x)
{
  return x-'0';
}

void *intal_add(void *intal1, void *intal2)
{
  if((intal1==NULL)||(intal2==NULL))
  {
		return intal_create("0");
  }

  char *str1=NULL;
  char *str2=NULL;
  int len2, len;
  if(intal_compare(intal1, intal2)>=0)
  {
      str1 = intal2str(intal1);
      str2 = intal2str(intal2);
      len = strlen(str1);
      len2 = strlen(str2);

  }
  else
  {
      str1 = intal2str(intal2);
      str2 = intal2str(intal1);
      len = strlen(str1);
      len2 = strlen(str2);

  }

  int p=len+1, i=0, j=0, k=0, flag=0;
  char *Res = (char*)calloc(len+2, sizeof(char));
  Res[0]='0';
  for(i=len-1; i>=0; i--)
  {
    Res[i+1]=str1[i];
  }
  Res[len+1]='\0';
  i=0;
  int carry=0;
  j=len;
  for(i=len2-1; i>-1; i--)
  {
    char x = Res[j];
    Res[j]=(con_int(Res[j])+con_int(str2[i])+carry)%10 + '0';
    carry = (con_int(x)+con_int(str2[i])+carry)/10;
    if(carry==1)
    {
      flag=0;
      k=j;
      while(Res[k-1]=='9')
      {
        flag=1;
        Res[k-1]='0';
        k--;
      }

      Res[k-1]=Inc_Dig(Res[k-1]);
      carry=0;
    }
    j--;
  }


  void *Intal=intal_create(Res);
  free(Res);
  free(str1);
  free(str2);
  return Intal;
}

int Sub_Digits(int x, int y)
{
	if(x>=y)
	{
		return (x-y);
	}
	else
	{
		return (10+x-y);
	}
}

void *intal_diff(void *intal1 ,void *intal2)
{
  if((intal1==NULL)||(intal2==NULL))
  {
    return intal_create("0");
  }

  char *str1 = NULL;
  char *str2 = NULL;
  int len, len2;
  if(intal_compare(intal1, intal2)>=0)
  {
      str1 = intal2str(intal1);
      str2 = intal2str(intal2);
      len = strlen(str1);
      len2 = strlen(str2);

  }
  else
  {
      str1 = intal2str(intal2);
      str2 = intal2str(intal1);
      len = strlen(str1);
      len2 = strlen(str2);

  }


  int flag, j, i;
  char k;

  for(i=0; i<len; i++)
  {
    if(i<len2)
    {
      k=str1[len-i-1];
      str1[len-i-1]=Sub_Digits(con_int(str1[len-i-1]), con_int(str2[len2-i-1]))+'0';
      if((con_int(k)-con_int(str2[len2-i-1]))<0)
      {
        j=i;
        while(str1[len-j-2]=='0')
        {
          str1[len-j-2]='9';
          j++;
        }
        str1[len-j-2]=Dec_Dig(str1[len-j-2]);
      }
    }
  }

  void *Intal = intal_create(str1);
  free(str1);
  free(str2);
  return Intal;
}

void *PowTen(void *intal, int n)
{
  char *str = intal2str(intal);
  char *Res = (char*)calloc(strlen(str)+n+1, sizeof(char));
  int i=0;
  for(i=0;  i<strlen(str)+n; i++)
  {
    if(i<strlen(str))
    {
      Res[i]=str[i];
    }
    else
    {
      Res[i]='0';
    }
  }
  Res[i]='\0';
  void *P = intal_create(Res);
  free(Res);
  free(str);
  return P;
}

void *Dig_Mul(char *Num, char x)
{
  int i=0, len=strlen(Num);
  int p=len+1;
  char carry='0';
  int d = x-'0';
  char *Res = (char*)calloc(p+1, sizeof(char));

  for(i=0; i<p; i++)
  {
    Res[i]='0';
  }

  Res[p]='\0';

//  printf("%s\n", Res);

  for(i=0; i<len; i++)
  {
    Res[p-i-1]=(((con_int(Num[len-i-1])*d)+con_int(carry))%10)+'0';
    carry = ((con_int(Num[len-i-1])*d)-con_int(Res[p-i-1])+con_int(carry))/10+'0';
  }
  Res[0]=carry;
  void *Intal = intal_create(Res);
  free(Res);
  return Intal;
}

void *intal_divide(void *intal1, void *intal2)
{
  void *Z = intal_create("0");
	if((intal1==NULL)||(intal2==NULL)||(intal_compare(intal2, Z)==0))
	{
		return Z;
	}
	if(intal_compare(intal2, intal1)==1)
	{
		return Z;
	}

	if(intal_compare(intal2, intal1)==0)
	{
    free(Z);
		return intal_create("1");
	}

  if(intal_compare(intal2, Z)==0)
  {
    free(Z);
    return intal_create("0");
  }

  char *str1 = intal2str(intal1);
  char *str2 = intal2str(intal2);
  void *Div = intal_create(str1);
  int exp1 = strlen(str1)-strlen(str2);
  void *Res = intal_create("0");
  void *One = intal_create("1");
  int flag=1;
  while(intal_compare(Div, intal2)>-1)
  {
    void *C = PowTen(intal2, exp1);
    if(intal_compare(Div, C)<0)
    {
      exp1--;
      intal_destroy(C);
    }
    else
    {
      void *A = PowTen(One, exp1);
      void *B = PowTen(intal2, exp1);
      void *X = Res;
      void *Y = Div;
      Res = intal_add(Res, A);
      Div = intal_diff(Div, B);
      free(X);
      free(Y);
      intal_destroy(A);
      intal_destroy(B);
      intal_destroy(C);
    }
  }
  free(Z);
  free(One);
  free(Div);
  free(str1);
  free(str2);
  return Res;
}


void *intal_multiply(void *intal1, void *intal2)
{
  if((intal1==NULL)||(intal2==NULL))
	{
		return intal_create("0");
	}
  void *Z =intal_create("0");
  if((intal_compare(intal1, Z)==0)||(intal_compare(intal2, Z)==0))
  {
    return Z;
  }
  free(Z);
  char *str1 = intal2str(intal1);
	char *str2 = intal2str(intal2);
  //printf("%s %s\n", str1, str2);
	int len1 = strlen(str1);
	int len2 = strlen(str2), m=0;

  if(len1<=1)
  {
    char X = str1[0];
    free(str1);
    void *Intal= Dig_Mul(str2, X);
    free(str2);
    return Intal;
  }

  if(len2<=1)
  {
    char X = str2[0];
    free(str2);
    void *Intal= Dig_Mul(str1, X);
    free(str1);
    return Intal;
  }

  int *result = (int*)calloc(len1+len2, sizeof(int));
  int i=0, j, x=0, y=0, carry, n1, n2, sum=0;
  for(i=len1-1; i>=0; i--)
  {
    carry = 0;
    n1 = str1[i]-'0';
    y=0;
    for(j=len2-1; j>=0; j--)
    {
      n2 = str2[j]-'0';
      sum = n1*n2 + result[x+y]+carry;
      carry = sum/10;
      result[x+y] =sum%10;
      y++;
    }
    if(carry>0)
    {
      result[x+y]+=carry;
    }
    x++;
  }
  char *Res = (char*)calloc(len1+len2+1, sizeof(char));
  for(i=0; i<len1+len2; i++)
  {
     Res[i]=result[len2+len1-i-1]+'0';
  }

  Res[len1+len2]='\0';
  free(result);
  void *Intal= intal_create(Res);
  free(Res);
  free(str1);
  free(str2);
  return Intal;
}

void* intal_pow(void* intal1, void* intal2)
{
  void *Z= intal_create("0");
  if(intal1==NULL || intal2==NULL)
	return Z;

  if(intal_compare(intal1, Z)==0)
  {
	return Z;
  }

  if(intal_compare(intal2, Z)==0)
  {
    free(Z);
    return intal_create("1");
  }
  free(Z);
  void *One = intal_create("1");
  if(intal_compare(intal2, One)==0)
  {
    free(One);
    return intal1;
  }
  
  void *nextNo = intal_create("1");
  void *Res = intal_create("1");

  void *T1, *T2;

  while (1)
  {
  	T1 = Res;
  	T2 = nextNo;
  	nextNo = intal_add(nextNo, One);
  	Res = intal_multiply(Res, intal1);
  	if(intal_compare(nextNo, intal2)==0)
  	{
  		free(One);
  		free(nextNo);
  		free(T1);
  		free(T2);
  		return Res;
  	}
  	free(T1);
  	free(T2);
  }
  
}
