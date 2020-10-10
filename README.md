##Cryptocurrency Mining Simulation Program


#Problem Definition

Dishonest mining is a serious problem in blockchain based cryptocurrencies. Malicious miners can exploit honest miners by conducting dishonest strategies such as selfish mining [1], block withholding [2], eclipse attack [3] and pool hopping [4] to name a few. 
Nojoumian et al. [5] proposed a new reputation-based paradigm to address this issue. This new framework relies on game theoretical concepts as it is designed to incentivize the miners to commit to honest mining strategies. In this new system, each miner is assigned a public and perpetual reputation value that is initially zero and it will change over time based on the behavior of the miner. The range of the value is between -1 (lowest) and 1 (highest). If miners commit a malicious activity and it is detected by the system, their reputation value are affected negatively. Likewise, the reputation value of miners who are committed to honest mining, will increase over time.


#Objective

This simulation is designed to evaluate the effectiveness of the mentioned reputation-based paradigm. This program essentially simulates the interaction between the miners, pools and the pool managers in two scenarios:

In the first scenario there is no reputation and the pool managers send invitation to miners solely based on their mining power. 
In the second scenario, the reputation score is also considered. So the miners with   lower reputations are less likely to be invited to join the mining pools.

To evaluate the effectiveness of the proposed framework, data will be extracted from the program and statistical analysis of that data will provide evaluation measurements.


#The Architecture

To simulate the cryptocurrency mining environment, the program consists of different entities that are defined as separate modules. These modules interact with one other. These interactions are initiated upon the execution of the program. The main modules included are:

Random Generator Module: This is the random generator engine of the whole program. The engine can generate a random value for all the necessary attributes. The statistical parameters for this module are provided by the parameter module.

Parameter Module: This module provides all the mining parameters including crypto unit price, average block generation duration, number of crypto units per new block, etc. These units can be provided through a real live API data provider or they can be customized inside the program user interface. The module also provides the probability distribution attributes that are used by the random generator engine.

Miner Module: Includes miners attributes, mining data and behavior definitions. The main attributes are name, ID number, mining power, electricity cost rate and dishonesty factor (a value that sets the probability of conducting a single dishonest strategy at every mining round). These values are generated randomly upon the creation of the miner’s object. The mining data includes, pool membership, collected rewards, total electricity costs and the reputation value. Behavior definition is a set of utility functions that computes the expected profit as the result of a given action. (such as joining a particular mining pool, or whether or not to commit a certain dishonest strategy)

Mining Pool Module: This module represents mining pool entities and contains:
Pool attributes including pool fee rate and desired total mining power for pools. 
The mining data including the list of member miners, total number of mined blocks, pool’s profit, and number of dishonest mining activities.
A designated pool manager that controls the actions of the mining pool such as employing and releasing individual miners based on a utility function similar to the miner’s utility function that computes the expected profit for a given action.
If a full proof-of-work is provided by a member miner, then after deducting the pool’s fee, the pool will distribute the rewards among the member miners proportionate to their hash power.

Pool Manager Module: This module controls the actions of the pool which the manager is assigned to manage. The pool manager includes a utility function that is aimed to maximize the profit for both the manager and the designated mining pool.

The Game Module: This module is a set of functions that operates the mining game. The mining game is a game of chance in which an individual miner successfully provides a full proof-of-work. The probability of the proof-of-work is based on the miners hash power. The module also generates a time duration for each mining round. This time duration will define the electricity cost for the participating mines. Then the miners profiles are updated with the newly added costs. The game module also includes a mechanism to detect miners’ violations. This detection mechanism relies on a probabilistic algorithm that inspects abnormalities among miners’ actions.

Population Module: This module is a singleton object that is the source of all entities including miner, pool managers and pools. All entities inside the program environment are first created by this module. When the program terminates, this module saves all the entity data and when the program executes, this module imports all the saved data back to the program. The population modules simulates the real population characteristics. For example, the miner population will change overtime, new fresh miners join and old miners leave the system. Particularly the ones who have lost money or haven’t gained a substantial profit after participating several number of mining rounds.

User Interface: The user interface provides the user real time data monitoring and visualization for the different aspects of the mining game. It also includes a setting pane in which, the user can adjust various program parameters.


#Program’s Workflow

	Initially the program generates a defined population of miners each with random attributes. The program also generates a number of mining pools and pool managers. The mining pools are initially empty. A random number is assigned to each pool that defines the desirable number of miners that the pool is willing to employ. Each miner has a utility function. This function enables miners to estimate the amount of reward they may collect as the result of selecting a particular mining pool. Miners have the option to choose a mining pools. For example, a miner evaluates a pool by the fee the pool charges and the pool’s hash power. This evaluation will determine which pool to join. Likewise the mining pools have their own utility function to determine which miners to employ.
After the initial steps, the mining games begin and the data for all the entities are updated as individual mining games take place. The population modules controls the activeness of all entities. For example, if a miner loses money after a certain number of mining games or its utility function determines a negative profit, it sends a signal to the population module and the population module will remove it from the active list of miners and it will be invisible to other entities, however, its data will remain in the system. The population module also keeps generating new miners and adds them to the active list of miners. The same procedures also exist for pools and pool managers but with less intensity.


#Implementation Details

	The main components and the core architecture of this program is developed inside Xcode IDE using C++ programing language. For better performance, this program is designed using a multi-threaded architecture. Various C++ standard libraries such as File Stream, Thread, Time, Random Math libraries are used.
For the user interface there are two options:
Juce version 6: Juce is a multi-platform C++ library that can provide the basic UI design platform for C++ based programs. It can be compiled for Windows, macOS and iOS. 
A web-based user-interface: The other option is to build a web user-interface using HTML, CSS and Javascript programing language. This interface can be accessed through the web.

#Reference

[1] Eyal, Ittay, and Emin Gün Sirer. "Majority is not enough: Bitcoin mining is vulnerable." International conference on financial cryptography and data security. Springer, Berlin, Heidelberg, 2014.

[2] Courtois, Nicolas T., and Lear Bahack. "On subversive miner strategies and block withholding attack in bitcoin digital currency." arXiv preprint arXiv:1402.1718 (2014).

[3] Heilman, Ethan, et al. "Eclipse attacks on bitcoin’s peer-to-peer network." 24th {USENIX} Security Symposium ({USENIX} Security 15). 2015.

[4] Singh, Sushil Kumar, et al. "Smart contract-based pool hopping attack prevention for blockchain networks." Symmetry 11.7 (2019): 941.

[5] Nojoumian, Mehrdad, et al. "Incentivizing blockchain miners to avoid dishonest mining strategies by a reputation-based paradigm." Science and Information Conference. Springer, Cham, 2018.
