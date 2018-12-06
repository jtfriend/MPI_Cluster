#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>

#include "../Original_Code/original_aes_functions.c"

int main (void)
{
  clock_t begin = clock();

  unsigned char *key =         "#####f04001#####";
  unsigned char possPass[17] = "#####000000#####";

  unsigned char *iv = (unsigned char *)"0123456789012345";
  unsigned char *plaintext = (unsigned char *)"The quick brown fox jumps over the lazy dog";

  unsigned char ciphertext[128];
  unsigned char new_ciphertext[128];
  unsigned char decryptedtext[128];
  unsigned char new_decryptedtext[128];

  int decryptedtext_len, ciphertext_len;

  /* Encrypt the plaintext */
  ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);

  printf("Ciphertext is:\n");
  // BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  //Parallel MPI Part

  int rank;
  int size;

  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // Print off a hello world message
  printf("Hello world from processor %s, rank %d out of %d processors\n",
          processor_name, world_rank, world_size);

  // int number;
  // if (world_rank == 0) {
  //     number = -1;
  //     MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  // } else if (world_rank == 1) {
  //     MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
  //             MPI_STATUS_IGNORE);
  //     printf("Process 1 received number %d from process 0\n",
  //           number);
  // }


  int escCount = 16;
  int error = 0;

  char *a = "0123456789abcdef";

  for (int n5=0; n5<escCount; n5++) {
    possPass[5] = a[n5];
    for (int n4=0; n4<escCount; n4++) {
      possPass[6] = a[n4];
      for (int n3=0; n3<escCount; n3++) {
        possPass[7] = a[n3];
        for (int n2=0; n2<escCount; n2++) {
          possPass[8] = a[n2];
          for (int n1=0; n1<escCount; n1++) {
            possPass[9] = a[n1];
            for (int n0=0; n0<escCount; n0++) {
              possPass[10] = a[n0];
              //compare calculating text with cipher text generated

              // printf("%s\n", possPass);
              ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), possPass, iv, new_ciphertext);
              if (strncmp(new_ciphertext, ciphertext, 16) == 0) {
                escCount = 0;
                decryptedtext_len = decrypt(ciphertext, ciphertext_len, possPass, iv, decryptedtext);
                /* Add a NULL terminator. We are expecting printable text */
                decryptedtext[decryptedtext_len] = '\0';

                /* Show the decrypted text */
                printf("Decrypted text is:\n");
                printf("%s\n", decryptedtext);

                clock_t endtime = clock();
                double time_spent = (double)(endtime - begin) / CLOCKS_PER_SEC;
                printf(" time: %f\n", time_spent);
                MPI_Abort(MPI_COMM_WORLD, 1);
              }
            }
          }
        }
      }
    }
  }

  MPI_Finalize();

  decryptedtext_len = decrypt(ciphertext, ciphertext_len, possPass, iv, decryptedtext);

  /* Add a NULL terminator. We are expecting printable text */
  decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  printf("Decrypted text is:\n");
  printf("%s\n", decryptedtext);

  clock_t endtime = clock();
  double time_spent = (double)(endtime - begin) / CLOCKS_PER_SEC;
  printf(" time: %f\n", time_spent);

  return 0;
}