# Developer notes

This is a memento and also help for developers who want to contribute and help the project.

## General architecture

The software divides in two major parts, the processor and the editor.
The processor is also assisted by a third element known the worker.

The processor is intended to work independently, and must be able to perform its function even if the editor is not created.
The processor operates in real-time condition, but neither the worker nor the editor are bound by real-time constraints.

These elements communicate using bidirectional FIFO pairs, implemented as ring buffers:
- Processor ↔ Editor
- Processor ↔ Worker

### The processor

The processor begins it lifetime stopped, and then it can be toggled between playing and stopped state.

While playing, the block processing is invoked at regular rate, otherwise it is not.
Depending on the host, the processor cannot be considered active at all times; therefore, the user interface should not wait indefinitely for replies expected from the processing routing of the synthesizer. The playback state can be checked for readyness, and the processor must have a `null` FIFO when inactive, so this should be checked.

If the processor must be interrupted for any reason, such as perform non-realtime side operations, one can acquire the player lock. While acquired, the processor does not underrun; it will produce the zero signal and discard the MIDI input.

### The worker

The worker operates in a thread which is parallel to the processor. It performs some functions which the processor should not perform, because these are either non-realtime or time-consuming.

In order to summon the worker when needed, the processor will write a message on the FIFO queue and then post the semaphore.
When the worker has accomplished the task, it will send a result on the FIFO queue oriented in the other direction.

It is summoned to accomplish these tasks:
- modifying the chip count or emulator, which performs non-realtime memory operations;
- making a measurement of envelope times for a given FM patch.

When it's the case of modifying chip settings, the worker will shortly acquire a lock on the audio processing, and apply the modification on the synthesizer.

### The editor

The editor allows to edit any current state and send it to the processor, and it can also receive elements of state from the processor and display them.

To initialize itself, the editor sends a full state request to the processor over the FIFO queue.
This initialization occurs on two occasions:
- the creation of the editor;
- the ready message from the processor, indicating it has finished setting up after a (re)start of its playback.

The processor will answer with a rafale of messages which includes, but does not limit to, the entire set of banks and programs.

When the processor receives external updates to its state, such as by parameters, it will notify the editor of these by sending small incremental messages.

For manipulation by the user interface, depending on the kind of editing performed, updating to the processor will be made either by FIFO queue messaging, or by writing a value to a parameter.

## State keeping

There exist multiple entities to the managed state, which are as follows.
1. the active set of banks and programs
2. the chip settings
3. the bank global settings
4. the parameters

The elements 1, 2 and 3 are used by both the processor and editor, and they both have their own copy.
The parameters concern the host to plugin communication, they also keep a value, and it's most often a copy of data stored as synthesizer internals.

For the correctness of all the kinds of state, it's required to keep this data in synchronization.
