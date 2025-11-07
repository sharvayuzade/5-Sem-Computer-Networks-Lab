#include <stdio.h>

int main() {
    int n, i, j, temp, current_time = 0, completed = 0;
    float avg_waiting_time = 0, avg_turnaround_time = 0, avg_completion_time = 0;
    
    // Dynamically get the number of processes from user
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    // Arrays to store process information
    int process_id[n];
    int arrival_time[n];
    int burst_time[n];
    int waiting_time[n];
    int turnaround_time[n];
    int completion_time[n];
    int remaining_time[n];
    int is_completed[n];
    
    // Initialize arrays
    for (i = 0; i < n; i++) {
        process_id[i] = i + 1;
        is_completed[i] = 0;
    }
    
    // Get arrival time and burst time for each process
    printf("\nEnter arrival time and burst time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &arrival_time[i]);
        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &burst_time[i]);
        remaining_time[i] = burst_time[i];
    }
    
    // Process scheduling
    current_time = 0;
    completed = 0;
    
    while (completed != n) {
        int shortest_job_idx = -1;
        int shortest_burst = 999999;
        
        // Find the process with shortest burst time among the arrived processes
        for (i = 0; i < n; i++) {
            if (arrival_time[i] <= current_time && is_completed[i] == 0) {
                if (burst_time[i] < shortest_burst) {
                    shortest_burst = burst_time[i];
                    shortest_job_idx = i;
                }
            }
        }
        
        // If no process is found, increment time
        if (shortest_job_idx == -1) {
            current_time++;
            continue;
        }
        
        // Execute the process
        current_time += burst_time[shortest_job_idx];
        completion_time[shortest_job_idx] = current_time;
        turnaround_time[shortest_job_idx] = completion_time[shortest_job_idx] - arrival_time[shortest_job_idx];
        waiting_time[shortest_job_idx] = turnaround_time[shortest_job_idx] - burst_time[shortest_job_idx];
        
        // Mark the process as completed
        is_completed[shortest_job_idx] = 1;
        completed++;
        
        // Update statistics
        avg_waiting_time += waiting_time[shortest_job_idx];
        avg_turnaround_time += turnaround_time[shortest_job_idx];
        avg_completion_time += completion_time[shortest_job_idx];
    }
    
    // Calculate average waiting time, turnaround time, and completion time
    avg_waiting_time /= n;
    avg_turnaround_time /= n;
    avg_completion_time /= n;
    
    // Sort processes by ID for display (optional)
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (process_id[j] > process_id[j + 1]) {
                // Swap process IDs
                temp = process_id[j];
                process_id[j] = process_id[j + 1];
                process_id[j + 1] = temp;
                
                // Swap arrival times
                temp = arrival_time[j];
                arrival_time[j] = arrival_time[j + 1];
                arrival_time[j + 1] = temp;
                
                // Swap burst times
                temp = burst_time[j];
                burst_time[j] = burst_time[j + 1];
                burst_time[j + 1] = temp;
                
                // Swap waiting times
                temp = waiting_time[j];
                waiting_time[j] = waiting_time[j + 1];
                waiting_time[j + 1] = temp;
                
                // Swap turnaround times
                temp = turnaround_time[j];
                turnaround_time[j] = turnaround_time[j + 1];
                turnaround_time[j + 1] = temp;
                
                // Swap completion times
                temp = completion_time[j];
                completion_time[j] = completion_time[j + 1];
                completion_time[j + 1] = temp;
            }
        }
    }
    
    // Display results
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               process_id[i], arrival_time[i], burst_time[i], 
               completion_time[i], waiting_time[i], turnaround_time[i]);
    }
    
    printf("\nAverage Waiting Time: %.2f", avg_waiting_time);
    printf("\nAverage Turnaround Time: %.2f", avg_turnaround_time);
    printf("\nAverage Completion Time: %.2f\n", avg_completion_time);
    
    return 0;
}
