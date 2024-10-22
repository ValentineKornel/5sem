// const EventEmitter = require('events');

// class MyEmitter extends EventEmitter {}

// const myEmitter = new MyEmitter();

// myEmitter.on('event', () => {
//     console.log('Произошло событие!');
// });

// myEmitter.emit('event');


class MyEventEmitter {
    constructor() {
        this.events = {};
    }

    on(event, listener) {
        if (!this.events[event]) {
            this.events[event] = [];
        }
        this.events[event].push(listener);
    }

    emit(event, data) {
        if (this.events[event]) {
            this.events[event].forEach(listener => listener(data));
        }
    }
}

const emitter = new MyEventEmitter();

emitter.on('greet', (data) => {
    console.log(`Hello, ${data}!`);
});

emitter.emit('greet', 'Valentine');



// using System;

// public class Publisher
// {
//     public delegate void Notify();  // Делегат для события
//     public event Notify OnNotify;   // Событие

//     public void DoSomething()
//     {
//         Console.WriteLine("Произошло действие!");
//         OnNotify?.Invoke();  // Генерация события
//     }
// }

// public class Subscriber
// {
//     public void OnNotified()
//     {
//         Console.WriteLine("Событие обработано!");
//     }
// }

// class Program
// {
//     static void Main()
//     {
//         Publisher publisher = new Publisher();
//         Subscriber subscriber = new Subscriber();
        
//         // Подписка на событие
//         publisher.OnNotify += subscriber.OnNotified;
        
//         publisher.DoSomething();
//     }
// }
