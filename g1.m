// Target sequence we want to evolve towards
:T [3 7 1 4] t! ;

// Initialize random brain - creates array of 4 random numbers
:I 
  [0 0 0 0] b!  // Create empty brain array
  4(             // Loop 4 times
    10 /K 48 -   // Get random number 0-9 from keyboard
    b /i ?!      // Store in brain array at current index
  )
;

// Calculate fitness - lower is better
:F 
  0 s!          // Initialize score
  4(            // For each number in sequence
    t /i ? n!   // Get target number
    b /i ? m!   // Get brain number
    n m - " *   // Get absolute difference
    s + s!      // Add to score
  )
  s .           // Return fitness score
;

// Mutate brain slightly
:M
  4(              // For each number in brain
    b /i ? k!     // Get current number
    /K 48 - 2 <   // 20% chance to mutate
    (             // If should mutate
      k 1 +       // Increase by 1
      10 % p!     // Keep within 0-9 range
      p b /i ?!   // Store back in brain
    )
  )
;

// Main evolution loop
:E
  T              // Initialize target
  I              // Create initial brain
  
  100(           // Run for 100 generations
    F            // Calculate fitness
    `Fitness: ` 
    s .          // Print current fitness
    /N
    
    s 0 = (      // Check if perfect solution found
      `Perfect!`
      /N
      0(/)       // Break loop
    )
    
    M            // Mutate brain for next generation
  )
;

// Instructions
`Enter 4 random digits 0-9 to create initial brain, then watch it evolve!`
/N
`Target sequence is: 3 7 1 4`
/N
E
