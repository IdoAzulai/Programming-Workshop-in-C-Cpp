CC=gcc
CXX_FLAGS= -std=c99 -Wextra -Wvla -Werror -g
tweets: tweets_generator.o linked_list.o markov_chain.o
	$(CC) $(CXX_FLAGS) linked_list.o markov_chain.o tweets_generator.o -o tweets_generator

snake: snakes_and_ladders.o linked_list.o markov_chain.o
	$(CC) $(CXX_FLAGS) linked_list.o markov_chain.o snakes_and_ladders.o -o snakes_and_ladders

linked_list.o: linked_list.h linked_list.c
	$(CC) $(CXX_FLAGS) -c  linked_list.c
markov_chain.o: markov_chain.h markov_chain.c
	$(CC) $(CXX_FLAGS) -c  markov_chain.c
tweets_generator.o: markov_chain.h linked_list.h tweets_generator.c
	$(CC) $(CXX_FLAGS) -c  tweets_generator.c
snakes_and_ladders.o: markov_chain.h linked_list.h snakes_and_ladders.c
	$(CC) $(CXX_FLAGS) -c  snakes_and_ladders.c


    #target_tweet: tweets_generator.c
    #
    #
    #
    #target_snl: snakes_and_ladders.c
    #snakes_and_ladders 1 3
    #
    #all: main.o b

    #snakes_and_ladders: snakes_and_ladders.c markov_chain.c markov_chain.h linked_list.c linked_list.h
    #	gcc -Wvla -Werror
    #
    #tweets_generator: tweets_generator.c markov_chain.c markov_chain.h linked_list.c linked_list.h
    #	gcc -Wvla -Werror


