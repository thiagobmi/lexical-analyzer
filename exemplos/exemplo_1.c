int age;
int *agePointer;
float salary = 55000.75;
char *greeting = "Hello, User!";
int bonus = 1000;
int totalSalary;

// Assigning values
age = 30;
agePointer = &age;
totalSalary = salary + bonus;
if (totalSalary > 60000)
{
	totalSalary += 10000;

	// Simple output
	printf("Greeting: %s\n", greeting);
	printf("Initial salary: %.2f\n", salary);
	printf("Total salary with bonus: %d\n", totalSalary);

	char character = 'A';
	printf("Character: %c\n", character);

	// Conditional logic
	if (age > 25)
	{
		printf("Age is greater than 25\n");
		if ((agePointer != NULL) && (*agePointer < 35))
		{
			printf("Age is less than 35 and pointer is not NULL\n");
		}
	}
}
char *name = "Alex"; // valid
// float name = "Alex"; //invalid

// Arithmetic expressions and assignments
float tax = 0.2 * salary;
float netSalary = salary - tax;
printf("Net salary after tax: %.2f\n", netSalary);

// More complex condition
if ((bonus + totalSalary) >= 60000 && (salary * 1.1) <= 60000)
{
	printf("Eligible for extra benefits\n");
}

// Using fgets for user input
printf("Enter a new greeting: ");
fgets(buffer, 50, stdin);
printf("New greeting: %s\n", buffer);

return 0;