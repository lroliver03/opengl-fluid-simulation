# Log

This is kind of a diary for the project. I want to report my progress here.

### 22 Nov 2023 - main

So, I have a simulation, it runs but it's far from functional. The particles apparently gain energy throughout the whole ordeal. The start of the simulation should look like an adiabatic expansion, but it just expands and preserves temperature, which makes no physical sense.

I think I've reached the point where I have to make more branches, so that's the next step.

Next up I'll be working on:

- Adding way more particles safely
- Better collision handling
- Choosing whether to calculated velocities from predicted positions or only calculate positions from velocities
- Parallelization with OpenMP

### 28 Nov 2023 - multithreading

Array operations (such as calculating pressure and velocity) and density calculations are parallelized with OpenMP in [`tests/`](./tests). Using 8 threads, the time reduction for array operations was good, although density calculations only become significantly faster if we're dealing with a large number of particles, which shouldn't be a problem.

On another note, I decided to implement the perfected Euler method to solve for each step of the fluid system. It shouldn't be too difficult to apply in C++ and it's compatible with the parallelization I've tested.