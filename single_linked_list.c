// Program for doing CRUD operations using single linked list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20
#define DATA_FILE "item_records.dat"
#define SORTED_FILE "sorted_items.dat"
#define NO_ITEMS_FOUND printf("No items found. \n")
#define ITEM_NOT_FOUND printf("Item not found. \n")

struct item
{
	char item_id[SIZE];
	char item_name[SIZE];
	float item_price;
};

struct node
{
	struct item item;
	struct node *next;
};

FILE *fp_items;
struct node *head = NULL;
struct node *current;
struct node *next_node;
struct item temporary_item;

void add_item();
void show_all_items();
void update_item(struct node *);
void delete_item(struct node *);
void sort_by_name();
void sort_by_price();

void save_file(const char *file_name);
void load_data_from_file();
void append_node(struct node *);
struct node *find_node(char *);

int main()
{
	int choice;
	char searching_id[SIZE];
	struct node *matching_node;
	load_data_from_file();
	while (1)
	{
		printf("1. Add item\n2. Show all items\n3. Update item\n4. Delete item\n5. Sort by name\n6. Sort by price\n7. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			add_item();
			save_file(DATA_FILE);
			break;
		case 2:
			show_all_items();
			break;
		case 3:
			printf("Enter Item Id to update: ");
			scanf("%s", searching_id);
			matching_node = find_node(searching_id);
			if(matching_node == NULL)
			{
				ITEM_NOT_FOUND;
				break;
			}
			else
			{
				update_item(matching_node);
			}
			save_file(DATA_FILE);
			break;
		case 4:
			printf("Enter Item Id to delete: ");
			scanf("%s", searching_id);
			matching_node = find_node(searching_id);
			if (matching_node == NULL)
			{
				ITEM_NOT_FOUND;
				break;
			}
			else
			{
				delete_item(matching_node);
			}
			save_file(DATA_FILE);
			break;
		case 5:
			sort_by_name();
			break;
		case 6:
			sort_by_price();
			break;
		case 7:
			exit(0);

		default:
			printf("Invalid choice.\n");

		}
	}
	return 0;
}

void load_data_from_file()
{
	struct item item_record;
	fp_items = fopen(DATA_FILE, "rb");
	if (!fp_items)
	{
		printf("File not found. ");
		return;
	}
	while (fread(&item_record, sizeof(struct item), 1, fp_items))
	{
		struct node *new_node = (struct node *)malloc(sizeof(struct node));
		new_node->item = item_record;
		new_node->next = NULL;
		append_node(new_node);
	}
	fclose(fp_items);
}

void append_node(struct node *new_node)
{
	if (head == NULL)
	{
		head = new_node;
		return;
	}
	struct node *current_node = head;
	while (current_node->next != NULL)
	{
		current_node = current_node->next;
	}
	current_node->next = new_node;
	
}

void save_file(const char *file_name)
{
	struct node *current_node = head;
	fp_items = fopen(file_name, "wb");
	if (!fp_items)
	{
		printf("Unable to save file. \n");
		return;
	}
	while (current_node != NULL)
	{
		fwrite(&current_node->item, sizeof(struct item), 1, fp_items);
		current_node = current_node->next;
	} 
	fclose(fp_items);
}

void add_item()
{
	struct node *new_node = (struct node *)malloc(sizeof(struct node));
	printf("Enter Item Id: ");
	scanf("%s", new_node->item.item_id);
	printf("Enter Item Name: ");
	scanf("%s", new_node->item.item_name);
	printf("Enter Item Price: ");
	scanf("%f", &new_node->item.item_price);
	new_node->next = NULL;
	append_node(new_node);
}

struct node *find_node(char *searching_id)
{
	struct node *current_node = head;
	while (current_node != NULL)
	{
		if (strcmp(current_node->item.item_id, searching_id) == 0)
		{
			return current_node;
		}
		current_node = current_node->next;
	}
	return NULL;
}

void show_all_items()
{
	struct node *current_node = head;
	if (head == NULL)
	{
		NO_ITEMS_FOUND;
		return;
	}
	while (current_node != NULL)
	{
		printf("Item Id: %s\nItem Name: %s\nItem Price: %.2f\n", current_node->item.item_id, current_node->item.item_name, current_node->item.item_price);
		printf("------------------------------------\n");
		current_node = current_node->next;
	}
}

void update_item(struct node * matching_node)
{
	printf("Enter item new name: ");
	scanf("%s", matching_node->item.item_name);
	printf("Enter item new price: ");
	scanf("%f", &matching_node->item.item_price);

	printf("Item updated successfully. \n");
}

void delete_item(struct node *matching_node)
{
	struct node **indirect = &head;
	while ((*indirect) != matching_node)
	{
		indirect = &((*indirect)->next);
	}
	*indirect = matching_node->next;
	free(matching_node);
	printf("Item deleted successfully. \n");
}

void sort_by_name()
{
	if (head == NULL)
	{
		NO_ITEMS_FOUND;
		return;
	}
	for (current = head; current != NULL; current = current->next)
	{
		for (next_node = current->next; next_node != NULL; next_node = next_node->next)
		{
			if (strcmp(current->item.item_name, next_node->item.item_name) > 0)
			{
				temporary_item = current->item;
				current->item = next_node->item;
				next_node->item =temporary_item;
			}
		}
	}
	printf("------Sorting list of items by name-----\n");
	show_all_items();
}

void sort_by_price()
{
	if (head == NULL)
	{
		NO_ITEMS_FOUND;
		return;
	}
	for (current = head; current != NULL; current = current->next)
	{
		for (next_node = current->next; next_node != NULL; next_node = next_node->next)
		{
			if (current->item.item_price > next_node->item.item_price)
			{
				temporary_item = current->item;
				current->item = next_node->item;
				next_node->item = temporary_item;
			}
		}
	} 
	save_file(SORTED_FILE);
	printf("-------Sorting list of items by price-----\n");
	show_all_items();

}


