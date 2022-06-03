#include <usart.c>
#include <paulLib.h>

typedef struct LOG{
  int second;
  int distance;
  float currentSpeed;
  int burst;
  int fuelReserve;
} log_t;


typedef struct node
{
    log_t val;
    struct node * next;
    
} node_t;

void push(node_t *head, log_t val){
  node_t *current = head;
  while(current->next !=NULL){
    current=current->next;
  }
current->next=(node_t *)malloc(sizeof(node_t));
current->next->val=val;
current->next->next=NULL;

}

void print_list(node_t *head){
  node_t *current = head;
  char buf[10];
  while (current!=NULL)
  {
    gcvt(current->val.currentSpeed,buf,2);
    // printf("%s is the string of the float\n",buf);
    printf("Second:%d\nDistance:%d\nCurrent Speed:%s\nBurst:%d\nFuel Reserve:%d\n",current->val.second,current->val.distance,buf,current->val.burst,current->val.fuelReserve);
    current=current->next;
  }
  
}