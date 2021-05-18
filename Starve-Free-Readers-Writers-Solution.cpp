semaphore rmutex = 1;  // mutex used for <Entry> and <Exit> section while reading
semaphore resource = 1; // semaphore used to prevent simultaneous read and write
semaphore fifoq = 1; // semaphore used design sarve-free readers writers solution by maintaing FIFO signalling of requests
int readcount = 0;  //  used to keep track of concurrent read operations

/*
semphore x = 1;
wait(x) -> will decrement the value of x by 1 and will not allow furthur access as long as x = 0
signal(x) -> will increment the value of x by 1.
*/

reader_task()
{

    <Entry Section> // reader entry section

    wait(fifoq);  // service request ordering, (wait for the turn)
    wait(rmutex); // preventing more than one access to below segment
    readcount++;  // new reader entered
    if(readcount == 1)
    {
        wait(resource);  // if 1st reader then prevent the shared resource from concurrent writing
    }
    signal(fifoq); // next reader in line can come
    signal(rmutex); // <Entry> task of current reader is complete

    <Critical Section> // shared resource

    // Reading is performed here

    <Exit Section> // reader exit section

    wait(rmutex); // mutual exclusion required for below segment, to avoid any race bw readers
    readcount--; // current reader is leavinng
    if(readcount == 0)
    {
        signal(resource); // if there are no more readers, resource is open for reading as well as writing
    }
    signal(rmutex); // task is done and next reader can access exit or entry section

}

writer()
{

    <Entry Section>  // writer entry section
    wait(fifoq); // to maintain request order in fifo manner.
    wait(resource); // exclusive access for writing is required
    signal(fifoq);  // next writer in queue could be processed

    <Critical Section> // shared resource

    // writing is performed here

    <Exit Section> // writers exit section
    signal(resource); // release the resource when writing is done

}

/*
In order to prevent the race conditon bw reader and writer fifoq is used which follows first in first out
(FIFO), request ordering. This will prevent starvation of both reader and writer.
In this concurrent read operation is allowed, but as sson as there are no reader and a writer is waiting for
its turn, writer will get access to the resource, thus preventing starvation.
*/