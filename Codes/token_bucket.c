#include <stdio.h>

int main() {
    int bucket_size, n, rate, tokens = 0, incoming, i;

    printf("Token Bucket Algorithm Simulation\n");
    printf("Enter bucket size (capacity): ");
    scanf("%d", &bucket_size);
    printf("Enter token generation rate: ");
    scanf("%d", &rate);
    printf("Enter number of packets (time units): ");
    scanf("%d", &n);

    for(i=0; i<n; i++) {
        printf("\nTime unit %d:\n", i+1);

        tokens += rate;
        if(tokens > bucket_size) tokens = bucket_size;

        printf("Enter number of packets to send: ");
        scanf("%d", &incoming);

        if(tokens >= incoming) {
            printf("Packets sent: %d\n", incoming);
            tokens -= incoming;
        } else {
            printf("Only %d tokens available, packets sent: %d, packets dropped: %d\n",
                    tokens, tokens, incoming - tokens);
            tokens = 0;
        }

        printf("Tokens left: %d\n", tokens);
    }

    printf("\nSimulation complete.\n");
    return 0;
}

