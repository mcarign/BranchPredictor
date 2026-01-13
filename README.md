# Branch Predictor Simulator

This branch prediction program was part of my Microprocessor Architecture (ECE 563) course at NCSU, where I was tasked with building a simulator that can predict branches using bimodal, gshare, and a hybrid prediction models of the two. This simulator does not simulate anything but the branch predictions and their actual outcomes to keep scope of the project focused on branch prediction. The program reads from a tracefile, each line of the file for each instruction passed inthat contains ```<hex_branch_PC> t|n```, where hex branch pc is the address of the branch instruction being passed through to the pipeline and t | n is the actual outcome for that branch, which I will use to compare with my branch predictor's predicted outcome. The measurements this simulator records are the number of predicitons, number of mispredictions, and misprediction rate.

(NOTE: When I mention the pipeline, the pipeline is not being simulated, it is just how we can think of where the branch instruction is moving through the CPU.)

## How to Execute the Program

There is a Makefile in the Project2/ folder that will compile the program properly, after compiliing the progam you can execute it by entering in the command line any of the following:
*  ```./sim bimodal <M2> <tracefile>``` for a bimodal simulation,
*   ```./sim gshare <M1 <N> <tracefile>``` for a gshare simulation,
*   or ```./sim hybrid <K> <M1> <N> <M2> <tracefile>``` for a hybrid simulation.

**DISCLAIMER:**  
This code is my own original work. It may not be used, copied, or adapted by anyone currently taking this course or by future students for their own project submissions. Doing so would violate the course’s academic integrity policies, which the professor has clearly stated. This repository is publicly available solely for the purpose of showcasing my work to recruiters, interviewers, and potential employers.

## Analysis

### Bimodal Predictor with Different Sizes on Three Different Trace Files

<img width="630" height="470" alt="image" src="https://github.com/user-attachments/assets/5707be05-7cf3-4b4b-9f12-d010dd1f8220" />

<img width="630" height="470" alt="image" src="https://github.com/user-attachments/assets/96ded63e-1322-484a-b364-0e5cc4d2310d" />

<img width="630" height="470" alt="image" src="https://github.com/user-attachments/assets/a643d8b2-2653-48a2-b642-ff82178d5860" />

| Benchmark | Min "m" at which mispred. rate reaches it min | Min Mispred. Rate |
|-----------|-----------------------------------------------|-------------------|
| gcc | m = 18 | 11.17% |
| jpeg | m = 13 | 7.59% |
| perl | m = 14 | 8.82% |

### GShare Predictor

<img width="989" height="590" alt="image" src="https://github.com/user-attachments/assets/103967a3-7873-4345-8b98-cc33b38e584d" />

| m | Smallest Global history length that yields lowest mispred. rate | Lowest Mispred. rate | Bimodal mispred. rate |
|---|-----------------------------------------------------------------|----------------------|-----------------------|
| 7 | 1 | 27.71% | 26.65% |
| 8 | 1 | 23.27% | 22.43% |
| 9 | 1 | 18.83% | 18.49% |
| 10 | 1 | 15.80% | 15.67% |
| 11 | 1 | 13.64% | 13.65% |
| 12 | 1 | 12.04% | 12.47% |
| 13 | 7 | 10.56% | 11.72% |
| 14 | 6 | 9.08% | 11.37% |
| 15 | 7 | 8.20% | 11.30% |
| 16 | 9 | 7.49% | 11.21% |
| 17 | 11 | 7.03% | 11.19% |
| 18 | 10 | 6.73% | 11.17% |
| 19 | 12 | 6.47% | 11.17% |
| 20 | 11 | 6.37% | 11.17% |
