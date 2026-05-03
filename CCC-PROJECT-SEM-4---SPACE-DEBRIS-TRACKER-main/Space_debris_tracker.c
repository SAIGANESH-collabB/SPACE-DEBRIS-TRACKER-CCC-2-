 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HASH_SIZE 100
#define MAX_DEBRIS 200
#define QUEUE_SIZE 200


typedef struct Debris {
    char id[10];
    float velocity;
    float altitude;
    float distance;
    float risk;
    struct Debris* next;
} Debris;

// KD-TREE NODE
typedef struct KDNode {
    Debris* data;
    struct KDNode* left;
    struct KDNode* right;
} KDNode;
// gloabl structure
Debris* head = NULL;               // Linked list head
Debris* hashTable[HASH_SIZE];      // Hash table
Debris* heap[MAX_DEBRIS];          // Max heap
KDNode* kdRoot = NULL;             // KD-tree root
Debris* queue[QUEUE_SIZE];         // Circular queue

int heapSize = 0;
int front = -1, rear = -1;

int hash(char* id) {
    int sum = 0;
    for (int i = 0; id[i]; i++) sum += id[i];
    return sum % HASH_SIZE;
}

float computeRisk(float v, float a, float d) {
    return (v * v) / (a * d);
}
//linked list 
void insertLinkedList(Debris* node) {
    node->next = head;
    head = node;
}

//hash table
void insertHash(Debris* node) {
    int index = hash(node->id);
    node->next = hashTable[index];
    hashTable[index] = node;
}
//max heap 
void heapifyUp(int index) {
    if (index == 0) return;
    int parent = (index - 1) / 2;
    if (heap[parent]->risk < heap[index]->risk) {
        Debris* temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;
        heapifyUp(parent);
    }
}

void insertHeap(Debris* node) {
    heap[heapSize] = node;
    heapifyUp(heapSize);
    heapSize++;
}

Debris* getHighestRisk() {
    if (heapSize == 0) return NULL;
    return heap[0];
}
// kd tree insertion
KDNode* insertKD(KDNode* root, Debris* data, int depth) {
    if (!root) {
        KDNode* node = (KDNode*)malloc(sizeof(KDNode));
        node->data = data;
        node->left = node->right = NULL;
        return node;
    }

    int cd = depth % 2;
    if ((cd == 0 && data->altitude < root->data->altitude) ||
        (cd == 1 && data->distance < root->data->distance))
        root->left = insertKD(root->left, data, depth + 1);
    else
        root->right = insertKD(root->right, data, depth + 1);

    return root;
}

// Search KD-tree within range
void searchKD(KDNode* root, float altC, float distR, int depth) {
    if (!root) return;
    Debris* d = root->data;

    if (fabs(d->altitude - altC) <= 50 || fabs(d->distance - distR) <= 500) {
        printf("ID: %s (Alt: %.1f, Dist: %.1f, Risk: %.4f)\n",
               d->id, d->altitude, d->distance, d->risk);
    }

    int cd = depth % 2;
    if ((cd == 0 && altC < root->data->altitude) ||
        (cd == 1 && distR < root->data->distance))
        searchKD(root->left, altC, distR, depth + 1);
    else
        searchKD(root->right, altC, distR, depth + 1);
}
//circular queue
void enqueue(Debris* d) {
    if ((rear + 1) % QUEUE_SIZE == front) return;
    if (front == -1) front = 0;
    rear = (rear + 1) % QUEUE_SIZE;
    queue[rear] = d;
}

Debris* dequeue() {
    if (front == -1) return NULL;
    Debris* d = queue[front];
    if (front == rear) front = rear = -1;
    else front = (front + 1) % QUEUE_SIZE;
    return d;
}
//collision detection
void detectCollision() {
    if (!head || !head->next) {
        printf("Not enough debris to check for collision.\n");
        return;
    }

    Debris* a = head;
    while (a) {
        Debris* b = a->next;
        while (b) {
            float dist = fabs(a->distance - b->distance);
            float velDiff = fabs(a->velocity - b->velocity);

            if (dist < 500 && velDiff < 0.5) {
                printf("Possible Collision Detected:\n");
                printf("%s & %s\n", a->id, b->id);
                printf("Distance Diff = %.2f km, Velocity Diff = %.2f km/s\n\n",
                       dist, velDiff);
            }
            b = b->next;
        }
        a = a->next;
    }
}
//display linked list 
void displayDebris() {
    Debris* t = head;
    printf("ID     Vel    Alt    Dist    Risk\n");
    printf("--------------------------------------\n");
    while (t) {
        printf("%s   %.2f   %.2f   %.2f   %.4f\n",
               t->id, t->velocity, t->altitude, t->distance, t->risk);
        t = t->next;
    }
}

int main() {
    int choice;

    while (1) {
        printf("         SPACE DEBRIS TRACKER\n");
        printf("1) Add Debris\n");
        printf("2) Display All Debris\n");
        printf("3) Highest Risk Debris\n");
        printf("4) Spatial Search\n");
        printf("5) Orbit Simulation\n");
        printf("6) Collision Detection\n");
        printf("7) Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Debris* d = (Debris*)malloc(sizeof(Debris));

            printf("Enter Debris ID: ");
            scanf("%s", d->id);
            printf("Velocity (km/s): ");
            scanf("%f", &d->velocity);
            printf("Altitude (km): ");
            scanf("%f", &d->altitude);
            printf("Distance (km): ");
            scanf("%f", &d->distance);

            d->risk = computeRisk(d->velocity, d->altitude, d->distance);
            d->next = NULL;

            insertLinkedList(d);
            insertHash(d);
            insertHeap(d);
            kdRoot = insertKD(kdRoot, d, 0);
            enqueue(d);

            printf("Debris added successfully.\n");
        }

        else if (choice == 2) displayDebris();

        else if (choice == 3) {
            Debris* top = getHighestRisk();
            if (top)
                printf("Highest Risk: %s (Risk %.4f)\n", top->id, top->risk);
            else
                printf("No debris available.\n");
        }

        else if (choice == 4) {
            float alt, dist;
            printf("Enter altitude center: ");
            scanf("%f", &alt);
            printf("Enter distance reference: ");
            scanf("%f", &dist);
            searchKD(kdRoot, alt, dist, 0);
        }

        else if (choice == 5) {
            int steps;
            printf("Simulation Steps: ");
            scanf("%d", &steps);

            for (int i = 0; i < steps; i++) {
                Debris* d = dequeue();
                if (!d) break;

                d->distance += d->velocity;  // Simple Euler step
                printf("Step %d: %s New Distance = %.2f km\n",
                       i + 1, d->id, d->distance);

                enqueue(d);
            }
        }

        else if (choice == 6) detectCollision();

        else if (choice == 7) break;

        else printf("Invalid choice.\n");
    }

    return 0;
}