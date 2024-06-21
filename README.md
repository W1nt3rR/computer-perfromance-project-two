# Performance of computer systems

### This is a program designed to benchmark the performance of your processor, to show how multi threading can improve performance.

# Getting started

### To get started you can follow the instructions bellow to setup and compile the benchmark program.
The reason we compile the program on each computer is to make use of **-march=native -mtune=native** compiler flags to tune the program to the host processor to squeeze the most performance out of it.

# MSYS2 with Visual Studio Code

This guide will help you set up MSYS2 to work with Visual Studio Code for C/C++ development.

## Prerequisites

-   [Visual Studio Code](https://code.visualstudio.com/)
-   [MSYS2](https://www.msys2.org/)

## Installation Steps

### 1. Install MSYS2

1. Download the MSYS2 installer from the [official website](https://www.msys2.org/).
2. Run the installer and follow the instructions to install MSYS2.

### 2. Update MSYS2

1. Open the MSYS2 terminal (MSYS2 MSYS).

2. Update the package database and core system packages with the following command:

    ```sh
    pacman -Syu
    ```

### 3. Install Required Packages

1. Open the MSYS2 terminal (MSYS2 MSYS).

2. Install the development tools and other required packages. For example, to install the GNU Compiler Collection (GCC) and make:

    ```sh
    pacman -S base-devel gcc mingw-w64-x86_64-toolchain
    ```

### 4. Set Up Environment Variables.

1. Add the MSYS2 binary paths to your system PATH. Open your environment variables settings and add the following paths (assuming default installation directory):

    ```
    C:\msys64\usr\bin
    ```

    ```
    C:\msys64\mingw64\bin
    ```

### 5. Run with VS Code Debug

# Results:
Results generaly show a preference for higher thread count, though there are a few exceptions:

### 1. Core count limitation

Most modern processors have multiple threads per core (AMD SMT, Intel Hyper-Threading), meaning they can make the core execute two workloads simultaneously, provided they do not overlap in what part of the core they use. In the **EPYC Milan 32-core** test, some functions suffer from this when going to 64 threads from 32 threads on a 32-core processor, some other functions prefer it, although not by much.

### 2. Power limit

Laptop processors we tested on sometimes show preference of lower core count. That can be explained by power or temeprature limits. When pushed really hard as in this case, meaning we use multiple cores at once, they are forced to lower their core clocks to stay in their power and/or temeprature budget. That makes lower core count have higher core frequencies and thus executing the program threads faster. Multithreading overhead is way worse in this case.

# CPUs tested:

### 1. EPYC Milan 30-core

    ======================================================= Speed Table ======================================================
    Function          1 Thread     2 Threads    4 Threads    8 Threads    16 Threads   32 Threads   60 Threads   Best

    sumSquares        2.5679s      1.7126s      1.1036s      0.7266s      0.4545s      0.3324s      0.2670s      60 Threads
    step2             2.4294s      1.5228s      1.0139s      0.6274s      0.4136s      0.3077s      0.2635s      60 Threads
    quartic           3.3440s      2.2244s      1.3754s      0.8508s      0.4980s      0.3815s      0.3149s      60 Threads
    powell            2.4152s      1.5628s      1.0496s      0.7342s      0.4626s      0.3550s      0.2974s      60 Threads
    rosenbrock        2.3722s      1.5778s      1.0822s      0.6456s      0.4392s      0.3207s      0.2709s      60 Threads
    dixonPrice        2.5324s      1.7150s      1.0290s      0.6715s      0.4315s      0.3204s      0.2680s      60 Threads
    schwefel1_2       3.1841s      2.0366s      1.3092s      0.8394s      0.5004s      0.3625s      0.2933s      60 Threads
    schwefel2_20      2.2057s      1.4618s      0.9645s      0.6337s      0.3999s      0.3082s      0.2598s      60 Threads
    schwefel2_21      2.2082s      1.4114s      0.9285s      0.6255s      0.3988s      0.3108s      0.2640s      60 Threads
    rastrigin         2.7278s      1.7804s      1.1828s      0.7376s      0.4518s      0.3408s      0.2746s      60 Threads
    griewank          4.2996s      2.7445s      1.6100s      0.9462s      0.5100s      0.3373s      0.2981s      60 Threads
    csendes           2.8443s      1.7816s      1.1916s      0.7332s      0.4571s      0.3326s      0.2785s      60 Threads
    colville          1.1684s      0.7125s      0.4288s      0.2992s      0.2123s      0.1839s      0.1777s      60 Threads
    easom             1.1301s      0.6746s      0.4032s      0.2496s      0.1644s      0.1625s      0.1592s      60 Threads
    michalewicz       1.6015s      1.0463s      0.6474s      0.4340s      0.2757s      0.2184s      0.2025s      60 Threads
    shekel            1.1881s      0.7050s      0.3960s      0.2909s      0.2116s      0.1818s      0.1799s      60 Threads
    schwefel2_4       2.3541s      1.5077s      0.9471s      0.6478s      0.4039s      0.3166s      0.2659s      60 Threads
    schwefel          2.6157s      1.6545s      1.0188s      0.6774s      0.4077s      0.2997s      0.2552s      60 Threads
    schaffer          2.9950s      1.6830s      1.2825s      0.7511s      0.4583s      0.3402s      0.2788s      60 Threads
    alpine            1.9471s      1.2641s      0.8264s      0.5534s      0.3383s      0.2662s      0.2338s      60 Threads
    ackley            2.0570s      1.3162s      0.8560s      0.5542s      0.3432s      0.2698s      0.2351s      60 Threads
    sphere            1.7228s      1.0728s      0.6829s      0.4664s      0.3088s      0.2572s      0.2270s      60 Threads
    schwefel2_22      1.7126s      1.0581s      0.6718s      0.4836s      0.3124s      0.2534s      0.2244s      60 Threads

    =============================================== Speedup Table ===============================================
    Function          1 Thread     2 Threads    4 Threads    8 Threads    16 Threads   32 Threads   60 Threads

    sumSquares        1.0x         1.5x         2.3x         3.5x         5.7x         7.7x         9.6x
    step2             1.0x         1.6x         2.4x         3.9x         5.9x         7.9x         9.2x
    quartic           1.0x         1.5x         2.4x         3.9x         6.7x         8.8x         10.6x
    powell            1.0x         1.5x         2.3x         3.3x         5.2x         6.8x         8.1x
    rosenbrock        1.0x         1.5x         2.2x         3.7x         5.4x         7.4x         8.8x
    dixonPrice        1.0x         1.5x         2.5x         3.8x         5.9x         7.9x         9.5x
    schwefel1_2       1.0x         1.6x         2.4x         3.8x         6.4x         8.8x         10.9x
    schwefel2_20      1.0x         1.5x         2.3x         3.5x         5.5x         7.2x         8.5x
    schwefel2_21      1.0x         1.6x         2.4x         3.5x         5.5x         7.1x         8.4x
    rastrigin         1.0x         1.5x         2.3x         3.7x         6.0x         8.0x         9.9x
    griewank          1.0x         1.6x         2.7x         4.5x         8.4x         12.7x        14.4x
    csendes           1.0x         1.6x         2.4x         3.9x         6.2x         8.6x         10.2x
    colville          1.0x         1.6x         2.7x         3.9x         5.5x         6.4x         6.6x
    easom             1.0x         1.7x         2.8x         4.5x         6.9x         7.0x         7.1x
    michalewicz       1.0x         1.5x         2.5x         3.7x         5.8x         7.3x         7.9x
    shekel            1.0x         1.7x         3.0x         4.1x         5.6x         6.5x         6.6x
    schwefel2_4       1.0x         1.6x         2.5x         3.6x         5.8x         7.4x         8.9x
    schwefel          1.0x         1.6x         2.6x         3.9x         6.4x         8.7x         10.3x
    schaffer          1.0x         1.8x         2.3x         4.0x         6.5x         8.8x         10.7x
    alpine            1.0x         1.5x         2.4x         3.5x         5.8x         7.3x         8.3x
    ackley            1.0x         1.6x         2.4x         3.7x         6.0x         7.6x         8.8x
    sphere            1.0x         1.6x         2.5x         3.7x         5.6x         6.7x         7.6x
    schwefel2_22      1.0x         1.6x         2.5x         3.5x         5.5x         6.8x         7.6x

    ============================================== Average Results Table ==============================================
    Function          1 Thread      2 Threads     4 Threads     8 Threads     16 Threads    32 Threads    60 Threads

    sumSquares        0.542278      0.549876      0.495443      0.54982       0.580449      0.572597      0.61571
    step2             0.0245297     0.0240492     0.0237897     0.0249686     0.0264669     0.0269978     0.027185
    quartic           20.8733       20.7306       20.7674       20.648        20.83         20.6875       22.1565
    powell            2.32145       4.61304       4.16267       3.7027        1.47506       1.51915       2.29387
    rosenbrock        68.7481       75.8538       72.1711       66.0276       64.4268       68.9031       77.7625
    dixonPrice        1.30167       1.08065       1.02933       1.19893       1.36557       1.34839       1.1064
    schwefel1_2       0.191769      0.188467      0.195513      0.174634      0.184691      0.193168      0.250944
    schwefel2_20      7.17804       5.64367       5.52445       5.12445       5.01284       6.55032       4.60043
    schwefel2_21      0.0962448     0.0949026     0.0916096     0.0877988     0.0844653     0.0863309     0.0897222
    rastrigin         110.003       117.408       116.908       104.952       110.838       103.493       101.651
    griewank          0.00599119    0.00615686    0.00916008    0.00826398    0.0120372     0.00857897    0.00693942
    csendes           8.92402e-09   7.98963e-09   8.92504e-09   9.09679e-09   9.63106e-09   1.03754e-08   1.19218e-08
    colville          5.40073e-05   5.75087e-05   5.91471e-05   4.8619e-05    4.36909e-05   5.71852e-05   5.04719e-05
    easom             -0.666672     -0.700008     -0.666669     -0.500014     -0.566669     -0.666672     -0.633333
    michalewicz       -4.48017      -4.4129       -4.4426       -4.39661      -4.50455      -4.40033      -4.43545
    shekel            6.70656       6.22387       6.6869        6.17619       7.18806       6.91718       5.76266
    schwefel2_4       0.0137984     0.013306      0.013393      0.0136817     0.0145762     0.0146917     0.0151978
    schwefel          -10947.5      -10364.1      -10369.5      -11681.7      -11867.6      -12565.5      -12929.3
    schaffer          18.7095       17.0781       16.9682       16.2374       16.3623       16.9212       17.0678
    alpine            0.305257      0.258145      0.262539      0.319193      0.261293      0.338631      0.288175
    ackley            14.7211       14.1305       14.3578       14.5151       13.5734       14.5136       13.7751
    sphere            0.00334333    0.00314795    0.00328771    0.00310019    0.00327185    0.00343105    0.0033471
    schwefel2_22      0.271222      0.266414      0.270329      0.264741      0.266261      0.268823      0.270509

    ================================================ Best Results Table ===============================================
    Function          1 Thread      2 Threads     4 Threads     8 Threads     16 Threads    32 Threads    60 Threads

    sumSquares        0.394333      0.351033      0.345705      0.389155      0.389986      0.398712      0.423521
    step2             0.0197779     0.0191712     0.0198858     0.0200306     0.0192687     0.0193611     0.0223755
    quartic           19.6219       19.0832       18.7294       19.627        19.8335       18.9467       19.7132
    powell            0.387848      0.47077       0.486776      0.443917      0.375839      0.588662      0.306388
    rosenbrock        52.7517       55.9249       53.4534       53.4687       53.0339       52.5376       55.3022
    dixonPrice        0.870178      0.84329       0.820699      0.858469      0.863621      0.863492      0.929415
    schwefel1_2       0.132878      0.137476      0.136513      0.123086      0.135474      0.145548      0.158331
    schwefel2_20      1.36154       1.25144       1.31587       1.20424       1.17892       1.1449        1.26911
    schwefel2_21      0.0775386     0.0801774     0.0795978     0.0699717     0.0679086     0.0730867     0.0696541
    rastrigin         67.0367       91.3067       59.9871       65.7434       71.0264       57.1111       68.3937
    griewank          0.00221424    0.00198654    0.00209757    0.00194326    0.00231767    0.00204731    0.00243505
    csendes           4.78832e-09   4.97949e-09   2.96316e-09   5.42658e-09   4.69686e-09   6.66357e-09   7.74566e-09
    colville          1.01726e-05   1.15108e-05   1.86475e-05   9.42783e-06   1.14945e-05   1.93612e-05   2.67835e-06
    easom             -1            -1            -1            -1            -1            -1            -1
    michalewicz       -4.68672      -4.68714      -4.6872       -4.68699      -4.68734      -4.68739      -4.68719
    shekel            10.5364       10.5364       10.5364       10.5364       10.5364       10.5364       10.5364
    schwefel2_4       0.0112313     0.00941553    0.0111943     0.0121383     0.0119423     0.012049      0.013181
    schwefel          -16847        -15515.5      -16487.7      -14532        -15266.3      -15698.7      -15245.4
    schaffer          13.7369       11.2623       12.459        11.6181       12.4366       13.4223       11.6204
    alpine            0.143654      0.133816      0.0895771     0.0755156     0.0551401     0.0701849     0.0470691
    ackley            12.0418       12.7449       11.1005       11.2805       11.0931       12.2275       10.2817
    sphere            0.00270277    0.00200888    0.00253723    0.00241477    0.00247983    0.00254718    0.00266895
    schwefel2_22      0.25357       0.207429      0.219363      0.227982      0.233507      0.231856      0.246743
