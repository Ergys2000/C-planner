#include <stdio.h>
#include <string.h>

struct Topic {
	char name[30];
	int mins_required;
	int deadline_day;
};
struct Time{
	int s_h;
	int s_m;
	int e_h;
	int e_m;
	int total;
};
struct Schedule{
	char names[8][20];
	int s_h[8];
	int s_m[8];
	int e_h[8];
	int e_m[8];
	int i;
};


// ------------------THESE FUNCTIONS ARE TO WRITE, READ AND SORT THE WORK YOU HAVE TO DO ------------------------------

void get_work(){
	void write_work(struct Topic topics[], int n);
	int n_topics;
	int i;
	struct Topic topics[15];

	printf("How many topics do you have to study this week: ");
	scanf("%d", &n_topics);

	for(i=0; i<n_topics; i++){
		printf("-> Topic %d : ", i+1);
		scanf("%s", topics[i].name);
		printf("\n\tHow much minutes do you need to work in this topic : ");
		scanf("%d", &topics[i].mins_required);
		printf("\n\tHow much days do you have until your deadline : ");
		scanf("%d", &topics[i].deadline_day);
	}
	write_work(topics,n_topics);
}
void write_work(struct Topic topics[], int n){
	int i;
	FILE *infile = fopen("data/work_needed.txt", "w");
	fprintf(infile, "%31s%15s%10s\n", "Name", "Work Minutes", "Deadline");
	for(i=0; i<n; i++){
		fprintf(infile, "Topic%3d : %20s%15d%10d\n", i+1, topics[i].name, topics[i].mins_required, topics[i].deadline_day);
	}
	fclose(infile);
}
void sort_work(struct Topic topics[], int n){
	int i, j, key;
	struct Topic temp;
	for(i=0; i<n-1; i++){
		key=i;
		for(j=i+1; j<n; j++){
			if(topics[j].deadline_day < topics[key].deadline_day)
				key = j;
		}
		temp = topics[i];
		topics[i] = topics[key];
		topics[key] = temp;
	}
}
int read_work(struct Topic topics[]){
	FILE *infile = fopen("data/work_needed.txt", "r");
	int i=0;

	// this reads the unneccesary parts
	fscanf(infile, "%*s %*s %*s %*s");
	while(fscanf(infile, "%*s %*d %*c") != EOF){
		fscanf(infile, "%s %d %d", topics[i].name, &topics[i].mins_required, &topics[i].deadline_day);
		i++;
	}
	fclose(infile);
	return i; // i is the number of tasks that we encountered in the file
}

// ----------------------------------------------------------------------------------------------------------------------

// -----------------THESE FUNCTIONS ARE TO READ AND WRITE THE AMOUNT OF FREE TIME YOU HAVE EACH DAY---------------------
void get_time(){
	int i;
	int s_h, s_m, e_h, e_m;
	FILE *infile = fopen("data/free_time.txt", "w");

	for(i=0; i<7; i++){
		printf("=>Day %d of the week : ", i+1);
		printf("\n\t- Type the hour on which your free time starts : ");
		scanf("%d%*c%d", &s_h, &s_m);
		printf("\n\t- Type the hour on which your free time ends : ");
		scanf("%d%*c%d", &e_h, &e_m);
		fprintf(infile, "Day %d : %10.2d:%.2d  -  %.2d:%.2d\n", i+1, s_h, s_m, e_h, e_m);
	}
	fclose(infile);
}
void read_time(struct Time days[]){
	int i;
	FILE *infile = fopen("data/free_time.txt", "r");
	for(i=0; i<7; i++){
		fscanf(infile, "%*s %*d %*c %d %*c %d %*c %d %*c %d", &days[i].s_h, &days[i].s_m, &days[i].e_h, &days[i].e_m);
		days[i].total = (days[i].e_h - days[i].s_h)*60 + (days[i].e_m - days[i].s_m);
	}
	fclose(infile);
}

// ---------------------THESE FUNCTIONS ARE TO INITIALIZE THE WEEK SCHEDULE,
//----------------------PLACE AN AMOUND OF WORK INTO THE SCHEDULE OF A DAY, PRINT AND WRITE THE SCHEDULE
//----------------------AND THE MOST IMPORTANT PART OF THE PROJECT THE ALGORITHM WHICH MAKES THE SCHEDULE
void init_schedules(struct Schedule schedules[]){
	int i, j;
	for(i=0; i<7; i++){
		for(j=0; j<8; j++){
			strcpy(schedules[i].names[j], "undefined");
			schedules[i].s_h[j] = 0; schedules[i].s_m[j] = 0;
			schedules[i].e_h[j] = 0; schedules[i].e_m[j] = 0;
		}
	}
}
void put_in_schedule(const char name[], int am_work, struct Schedule *day_schedule, int j){
	int s_t;
	if(j>0){
		s_t = day_schedule->e_h[j-1]*60 + day_schedule->e_m[j-1];
		day_schedule->s_h[j] = day_schedule->e_h[j-1];
		day_schedule->s_m[j] = day_schedule->e_m[j-1];
	}
	else
		s_t = day_schedule->s_h[j]*60 + day_schedule->s_m[j];

	int e_t = s_t + am_work;
	strcpy(day_schedule->names[j], name);
	day_schedule->e_h[j] = e_t / 60;
	day_schedule->e_m[j] = e_t - day_schedule->e_h[j]*60;
}
void print_schedule(struct Schedule schedules[]){
	int i, j;
	for(i=0; i<7; i++){
			printf("\n==================Day %d==================\n", i+1);
			for(j=0; j<8; j++){
				if(schedules[i].e_h[j] != 0 || schedules[i].e_m[j] != 0)
					printf("%15s : %5.2d:%.2d - %.2d:%.2d\n", schedules[i].names[j], schedules[i].s_h[j], schedules[i].s_m[j],
							schedules[i].e_h[j], schedules[i].e_m[j]);
			}

		}
}
void write_schedule(struct Schedule schedules[]){
	int i, j;
	FILE *infile = fopen("data/Schedule.txt", "w");
	for(i=0; i<7; i++){
			fprintf(infile, "\n================== Day %d ==================\n", i+1);
			for(j=0; j<8; j++){
				if(schedules[i].e_h[j] != 0 || schedules[i].e_m[j] != 0)
					fprintf(infile, "%15s : %5.2d:%.2d - %.2d:%.2d\n", schedules[i].names[j], schedules[i].s_h[j], schedules[i].s_m[j],
							schedules[i].e_h[j], schedules[i].e_m[j]);
			}

	}
	fclose(infile);
}
int rnd(double a){
	return a/1 + 1;
}
void make_schedule(struct Topic topics[], struct Time days[], struct Schedule schedules[], int n_tasks){
	int i, j, k; // j -> used to keep track of the task you're working on the moment
				 // k -> used to keep track of the number of tasks you enter in one day
				 // i -> used to keep track of the day number
	double avg;  // avg -> the average amount of work for one task to be completed before the deadline
	int free_time; // -> the free time in minutes in one day
	for(i=0; i<7; i++){
		j=0; k = 0;
		free_time = days[i].total;
		schedules[i].s_h[0] = days[i].s_h; // initialize when the work starts in one day
		schedules[i].s_m[0] = days[i].s_m;
		while(free_time > 20){
			if(j >= n_tasks) break;
			avg = (double) topics[j].mins_required/(topics[j].deadline_day - i);
			if(avg <= 5 || topics[j].deadline_day-j == 0)
				j++;
			else if(avg <= free_time){
				put_in_schedule(topics[j].name, rnd(avg), &schedules[i], k);
				topics[j].mins_required -= rnd(avg);
				free_time -= rnd(avg);
				j++; k++;
			}else if(avg > free_time){
				put_in_schedule(topics[j].name, free_time, &schedules[i], k);
				free_time = 0;
				topics[j].mins_required -= free_time;
			}
		}
	}
}

// ------------------------THIS FUNCTIONS SHOWS THE ORDER OF HOW FUNCTIONS SHOULD BE RUN ------------------------------------
void Planner(int mode){
	// The Planner works when you have all the free time and work needed specified
	// you may have them specified on their respected files and not need to specify them again
	// thats why there are modes
	// mode = 0 : means that nothing is specified and so ask for all info
	// mode = 1 : means that the time is specified, only ask for work
	// mode != 1, 0 : means that time and work are specified, the number of tasks will be obtained when reading the file
	struct Topic tasks[15];
	struct Time days[7];
	struct Schedule schedules[7];
	int n;
	if(mode == 0){
		get_work();
		get_time();
		n = read_work(tasks);	
	}else if(mode == 1){
		get_work();
		n = read_work(tasks);
	}else
		n = read_work(tasks);

	sort_work(tasks, n);

	read_time(days);

	init_schedules(schedules);
	make_schedule(tasks, days, schedules, n);
	write_schedule(schedules);
	print_schedule(schedules);
}
