#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cmath>

typedef struct header
{
  char riff[4];
  float fsize;
  char file_type[4];
  char format_chunk[4];
  int chunk_size;
  short int format_type;
  short int channel_number;
  int samples_per_second;
  int bits_per_second;
  short int audio_type;
  short int bits_per_sample;
  char data[4];
  int data_length;
}hdr;

int hcf(long long int a,long long int b) // Highest common factor of a and b
{
  int t;

  while(b != 0)
  {
    t = b;
    b = a % b;
    a = t;
  };
  return a;
}


int modulo(unsigned long long a, unsigned long long b, unsigned long long c) { // Calculate (a^b)%c without calculating a^b
	unsigned long long n, *pows, *indexes, indexCounter = 0, searchBit = 1, partialMul = 1;
	n = floor(log2(b)) + 1;
	pows = new unsigned long long[n];
	indexes = new unsigned long long[n];
	pows[0] = a % c;
	for (int i = 1; i < n; i++) {
		pows[i] = (pows[i - 1] * pows[i - 1]) % c;
	}


	for (int i = 0; i < n; i++) {
		int index = b & searchBit;
		if (index != 0) {
			indexes[indexCounter] = floor(log2(index));
			indexCounter++;
		}
		searchBit = searchBit << 1;
	}

	for (int i = 0; i < indexCounter; i++) {
		partialMul = (partialMul * pows[indexes[i]]) % c;
	}
	delete pows;
	delete indexes;
	return partialMul % c;
}

int main(int argc, char *argv[])
{

  //    Handling arguments  
  
	int read = 0;
	std::string stre = "-encode";
	std::string strd = "-decode";
	std::string output = "-o";
	std::string name;
	
	if(argc != 3 && argc != 5)
	  {
	    std::cout<<"Error: wrong number of input arguments.\nPlease use ./[Prog_name] [file_name.wav] [-encode/-decode] \n(optional)[-o file_name.wav](default: sound2.wav)\n\n";
	    return 0;
	  }
	
	if(strstr(argv[1],".") == NULL)
	  {
	    std::cout<<"Error: please specify a .wav (WAVE) file with full extension.\n\n";
	    return 0;
	  }

	if(strcmp(strrchr(argv[1],'.')+1, "wav") != 0)
	  {
	    std::cout<<"Error: please specify a .wav (WAVE) file with full extension.\n\n";
	    return 0;
	  }
	
	if(argv[2] != stre && argv[2] != strd)
	  {
	    std::cout<<"Error: please specify the type of operation [-encode/-decode]\n\n";
	    return 0;
	  }

	if(argc > 3)
	  {
	    if(argv[3] == output)
	      {
		if(strstr(argv[4],".") == NULL)
			  {
			    std::cout<<"Error: please specify a .wav (WAVE) file with full extension.\n\n";
			    return 0;
			  }

		else if(strcmp(strrchr(argv[4],'.')+1, "wav") != 0)
		  {
		    std::cout<<"Error: please specify a .wav (WAVE) file with full extension.\n\n";
		    return 0;
		  }
        
		std::string name = argv[4];
	      }
	    else
	      {
		  std::cout<<"Error: wrong form of input arguments.\nPlease use ./[Prog_name] [file_name.wav] [-encode/-decode] \n(optional)[-o file_name.wav](default: sound2.wav)\n\n";
		  return 0;
	      }
	  } // if(argc>3)
	
	if(argc != 5)
	  std::string name = "sound2.wav";
        
	std::string operation_type = argv[2];
		
	hdr fheader;
	int fheader_size=sizeof(fheader);

	FILE *file;	
	FILE *info = fopen("../tmp/info.dat", "wb+");

	if((file=fopen(argv[1], "rb")) == NULL)
	  {
	    printf("Error: can't open sound file\n\n");
	    return 0;
	  }

	// Header reading
	
	read = fread(&fheader, fheader_size, 1, file);
	int file_length=sizeof(file);


	// Header display

	std::cout<<">>> Header information in file "<<argv[1]<<": <<<\n\n";
	
	printf("RIFF: \t\t\t");
	fprintf(info, "RIFF: \t\t\t");
        
	printf("%.4s", fheader.riff);
	fprintf(info ,"%.4s", fheader.riff);
		
	printf("\nFile type: \t\t");
	fprintf(info, "\nFile type: \t\t");
		
	printf("%.4s", fheader.file_type);
	fprintf(info, "%.4s", fheader.file_type);
		
	printf("\nFormat chunk: \t\t");
	fprintf(info, "\nFormat chunk: \t\t");
        
	printf("%.4s", fheader.format_chunk);
	fprintf(info, "%.4s", fheader.format_chunk);
	printf("\nData: \t\t\t");
	fprintf(info, "\nData: \t\t\t");
	
	printf("%.4s", fheader.data);
	fprintf(info,"%.4s", fheader.data);
	
	printf("\n");
	fprintf(info, "\n");
	
	printf("Frequency: \t\t%i Hz\n", fheader.samples_per_second);
	printf("Bits per second: \t%i\n", fheader.bits_per_second);
	printf("Number of channels: \t%i\n", fheader.channel_number);
	printf("Chunk size: \t\t%i\n", fheader.chunk_size);
	printf("Data length: \t\t%i KB\n", fheader.data_length/1024);
	fprintf(info, "Frequency: \t\t%i Hz\n", fheader.samples_per_second);
	fprintf(info, "Bits per second: \t%i\n", fheader.bits_per_second);
	fprintf(info, "Number of channels: \t%i\n", fheader.channel_number);
	fprintf(info, "Chunk size: \t\t%i\n", fheader.chunk_size);
	fprintf(info, "Data length: \t\t%i KB\n", fheader.data_length/1024);
	
	long sample_size = (fheader.channel_number * fheader.bits_per_sample)/8;
	fclose(info);
	
	if(fheader.format_type == 1)
	  {
	    long i = 0;
	    char data_buffer[sample_size];
	    int  size_is_correct = 1;
	    long bytes_in_each_channel = (sample_size / fheader.channel_number);
	    long num_samples = (8 * fheader.data_length) / (fheader.channel_number * fheader.bits_per_sample);
	    
	    
	    FILE *ofile;
	    ofile = fopen("output.wav", "wb");
	    fwrite(&fheader, sizeof(fheader), 1, ofile);
	    
	    if (size_is_correct)
	      { 
		if(operation_type == "-encode")
		  std::cout<<"Operation type: encoding\n";
		else
		  std::cout<<"Operation type: decoding\n";
		
		int data = 0;
	      
		// RSA algorithm: 
		std::cout<<"\n>>> RSA algorithm: <<<\n\n";
		int p = 46457;
		int q = 46133;
		unsigned long long int euler = (p-1) * (q-1);
		unsigned long long int n = p * q;
		int e = 2;
		while (hcf(euler, e)!=1 && e < n)
		  {
		    e++;
		  }
		long long int d = 2;
		
		int a,b,u,w,x,z,qq;
		a=e;
		b=euler;
	      
		u = 1; w = a;
		x = 0; z = b;

		// Calculating reverse modulo (d)
		while(w)
		  {
		    if(w < z)
		      {
			qq = u; u = x; x = qq;
			qq = w; w = z; z = qq;
		      }
		    qq = w / z;
		    u -= qq * x;
		    w -= qq * z;
		  }
		if(z == 1)
		  {
		    if(x < 0) x += b;
		    d=x;
		  }
		else
		  {
		    std::cout<<"Error: couldn't find reversed modulo\n\n";
		    return 0;
		  }
	      
		printf("p = %i\nq = %i\neuler = %lli\nn = %lli\ne = %i\nd = %lli\n", p, q, euler, n, e, d);
	      
		//  Reading data:
		

		long long int op_int;
		if(operation_type == stre)
		  op_int = e;
		else if(operation_type == strd)
		  op_int = d;
		else
		  {
		    std::cout<<"Error: wrong operation type\n\n";
		    return 0;
		  }
		
		for (i = 1; i <= num_samples; i++)
		  {
		    read = fread(&data, sizeof(data), 1, file);
		    
		    
		    if(data<0)
		      {
			data*=-1;
			data=modulo(data,op_int,n);
			data*=-1;
		      }
		    else
		      data=modulo(data,op_int,n);
		    
		    // Writing data:
		    
		    fwrite(&data, sizeof(data), 1, ofile);
		  } // for(i = 1...
	      } // if(size_is_correct)
	  } // if(fheader.format_type == 1)
		if(operation_type == "-encode")
		  std::cout<<"\n>>> Successfully encoded <<<\n";
		else
		  std::cout<<"\n>>> Successfully decoded <<<\n";
	return 0;
}
