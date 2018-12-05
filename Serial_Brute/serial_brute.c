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
  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */

  /* A 256 bit key */
//   unsigned char *key = (unsigned char *)"#####010005#####";
  unsigned char *key =         "#####006007#####";
  unsigned char possPass[17] = "#####000000#####";
//   key[17] = '\0';

  /* A 128 bit IV */
  unsigned char *iv = (unsigned char *)"0123456789012345";


  /* Message to be encrypted */
  unsigned char *plaintext = (unsigned char *)"The quick brown fox jumps over the lazy dog";

  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */
  unsigned char ciphertext[128];
  unsigned char new_ciphertext[128];

  unsigned char calculatingtext[128];

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];
  unsigned char new_decryptedtext[128];

  int decryptedtext_len, ciphertext_len;

  /* Encrypt the plaintext */
  ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);

  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");

  // BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  /* Decrypt the ciphertext */

  int escCount = 16;

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
              }
            }
          }
        }
      }
    }
  }

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

