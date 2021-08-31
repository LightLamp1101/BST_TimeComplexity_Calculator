#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TreeSize 100

struct bst_node {
	int data;
	struct bst_node* left;
	struct bst_node* right;
};

struct avl_node
{
	int data;
	int height;
	struct avl_node* left;
	struct avl_node* right;
};

struct result_t
{
	double bst_sum_height;
	double bst_sum_count;
	double avl_sum_height;
	double avl_sum_count;
};


//int max(int x, int y);
struct bst_node* bst_newnode(int n);
struct bst_node* bst_insert(struct bst_node* root, int data);
struct avl_node* avl_newnode(int data);
int avl_height(struct avl_node* node);
int avl_balance_factor(struct avl_node* node);
struct avl_node* avl_left_rotate(struct avl_node* node);
struct avl_node* avl_right_rotate(struct avl_node* node);
struct avl_node* avl_insert(struct avl_node* root, int data);
int bst_findheight(struct bst_node* root);
struct bst_node* bst_search(struct bst_node* root, int data);
int avl_findheight(struct avl_node* root);
struct avl_node* avl_search(struct avl_node* root, int data);
void make_random_array(int* array);
int delete_bst_nodes(struct bst_node* root);
int delete_avl_nodes(struct avl_node* root);
struct result_t* play_hundred_times(struct bst_node* bst_root, struct avl_node* avl_root);

int bst_search_cnt = 0;
int avl_search_cnt = 0;

int main()
{
	struct bst_node* bst_root = NULL;
	struct avl_node* avl_root = NULL;
	struct result_t* result;
	
	srand((unsigned int)time(NULL));
	
	result = play_hundred_times(bst_root, avl_root);

	printf("\n\n\n==========================\n %d개의 자료 비교\n bst height : %.2f, bst count : %.2f, avl height : %.2f, avl count : %.2f\n", 
		TreeSize, result->bst_sum_height/100.0, result->bst_sum_count/100.0, result->avl_sum_height/100.0, result->avl_sum_count/100.0);

	free(result);
	
	return 0;
}


//int max(int x, int y)
//{
//	return(x > y ? x : y);
//}

//Find Height of a binary Search Tree.
struct bst_node* bst_newnode(int data)
{
	struct bst_node* temp;
	temp = (struct bst_node*)malloc(sizeof(struct bst_node));
	temp->data = data;
	temp->left = temp->right = NULL;

	return temp;
}

struct bst_node* bst_insert(struct bst_node* root, int data)
{
	if (root == NULL)
		root = bst_newnode(data);

	else if (root->data >= data)
		root->left = bst_insert(root->left, data);

	else
		root->right = bst_insert(root->right, data);

	return root;
}

//Insertion and deletion in AVL Tree
struct avl_node* avl_newnode(int data)
{
	struct avl_node* temp = (struct avl_node*)malloc(sizeof(struct avl_node));
	temp->data = data;
	temp->left = NULL;
	temp->right = NULL;
	temp->height = 1;
	return temp;
};

int avl_height(struct avl_node* node)
{
	if (node == NULL)
		return 0;

	return node->height;
}

int avl_balance_factor(struct avl_node* node)
{
	if (node == NULL)
		return 0;

	return avl_height(node->left) - avl_height(node->right);
}

struct avl_node* avl_left_rotate(struct avl_node* node)
{
	struct avl_node* right_node = node->right;

	node->right = right_node->left;
	right_node->left = node;

	node->height = max(avl_height(node->left), avl_height(node->right)) + 1;
	right_node->height = max(avl_height(right_node->left), avl_height(right_node->right)) + 1;

	return right_node;
}

struct avl_node* avl_right_rotate(struct avl_node* node)
{
	struct avl_node* left_node = node->left;

	node->left = left_node->right;
	left_node->right = node;

	node->height = max(avl_height(node->left), avl_height(node->right)) + 1;
	left_node->height = max(avl_height(left_node->left), avl_height(left_node->right)) + 1;

	return left_node;
}

struct avl_node* avl_insert(struct avl_node* root, int data)
{
	if (root == NULL)
		return avl_newnode(data);

	if (data < root->data)
		root->left = avl_insert(root->left, data);

	else if (data > root->data)
		root->right = avl_insert(root->right, data);

	else
		return root;

	root->height = max(avl_height(root->left), avl_height(root->right)) + 1;

	int balance = avl_balance_factor(root);

	// Left Left Case
	if (balance > 1 && data < root->left->data)
		return avl_right_rotate(root);

	// Right Right Case
	if (balance < -1 && data > root->right->data)
		return avl_left_rotate(root);

	//Left Right Case
	if (balance > 1 && data > root->left->data)
	{
		root->left = avl_left_rotate(root->left);
		return avl_right_rotate(root);
	}

	// Right Left Case
	if (balance < -1 && data < root->right->data)
	{
		root->right = avl_right_rotate(root->right);
		return avl_left_rotate(root);
	}

	return root;
}


int bst_findheight(struct bst_node* root)
{
	if (root == NULL)
		return -1;

	return((max(bst_findheight(root->left), bst_findheight(root->right))) + 1);
}

struct bst_node* bst_search(struct bst_node* root, int data)
{
	if (root == NULL)
		return NULL;

	bst_search_cnt++;

	if (root->data == data)
		return root;
	else if (root->data > data)
		return bst_search(root->left, data);

	else
		return bst_search(root->right, data);
}


int avl_findheight(struct avl_node* root)
{
	if (root == NULL)
		return -1;

	return((max(avl_findheight(root->left), avl_findheight(root->right))) + 1);
}

struct avl_node* avl_search(struct avl_node* root, int data)
{
	if (root == NULL)
		return NULL;

	avl_search_cnt++;

	if (root->data == data)
		return root;
	else if (root->data > data)
		return avl_search(root->left, data);

	else
		return avl_search(root->right, data);
}

void make_random_array(int* arr) {

	int i, sub_i;

	for (i = 0; i < TreeSize; i++) {
		arr[i] = rand() % TreeSize + 1;
		for (sub_i = 0; sub_i < i; sub_i++) {
			if (arr[i] == arr[sub_i]) {
				i--;
				break;
			}
		}
	}
}

int delete_bst_nodes(struct bst_node* root)
{
	int count = 0;

	if (!root) {
		return 0;
	}

	if (root->left)
		count += delete_bst_nodes(root->left);

	if (root->right)
		count += delete_bst_nodes(root->right);

	free(root);
	root = NULL;

	return ++count;
}

int delete_avl_nodes(struct avl_node* root)
{
	int count = 0;

	if (!root) {
		return 0;
	}

	if (root->left)
		count += delete_avl_nodes(root->left);

	if (root->right)
		count += delete_avl_nodes(root->right);

	free(root);
	root = NULL;

	return ++count;
}

struct result_t* play_hundred_times(struct bst_node* bst_root, struct avl_node* avl_root) {
	struct result_t* result = malloc(sizeof(struct result_t));
	int arr[TreeSize];
	int i, j;
	int specific_num;

	memset(result, 0, sizeof(struct result_t));

	for (i = 0; i < 100; i++) {
		bst_root = NULL;
		avl_root = NULL;

		make_random_array(arr);

		//insert tree
		for (j = 0; j < TreeSize; j++) {
			bst_root = bst_insert(bst_root, arr[j]);
			avl_root = avl_insert(avl_root, arr[j]);
		}

		//search tree
		specific_num = rand() % TreeSize + 1;
		printf("%d spec %d\n\n", i, specific_num);
		bst_search(bst_root, specific_num);
		avl_search(avl_root, specific_num);

		//print value
		printf("%d번째 bst_Height = %d \n", i, bst_findheight(bst_root));
		printf("%d번째 bst_search cnt = %d \n", i, bst_search_cnt);
		printf("%d번째 avl_Height = %d \n", i, avl_findheight(avl_root));
		printf("%d번째 avl_search cnt = %d \n", i, avl_search_cnt);
		printf("\n\n");

		//accumulate value
		result->bst_sum_height += bst_findheight(bst_root);
		result->bst_sum_count += bst_search_cnt;
		result->avl_sum_height += avl_findheight(avl_root);
		result->avl_sum_count += avl_search_cnt;

		//init count
		bst_search_cnt = 0;
		avl_search_cnt = 0;

		//delete nodes
		delete_bst_nodes(bst_root);
		delete_avl_nodes(avl_root);
	}

	return result;
}