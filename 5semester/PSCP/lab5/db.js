const events = require('events')

let db = {
    data: [],

    select: async function(){
        return this.data
    },
    insert: async function(item){
        this.data.push(item);
    },
    update: async function(newItem){
        let index = this.data.findIndex(el => el.id == newItem.id);
        if (index !== -1) {
            this.data[index].name = newItem.name;
            this.data[index].bday = newItem.bday;
            return this.data[index];
        } else {
            return null;
        }
    },
    delete: async function(id){
        let index = this.data.findIndex(el => el.id == id);
        if (index !== -1) {
            let deletedItem = this.data.splice(index, 1)[0];
            return deletedItem;
        } else {
            return null;
        }
    },
    commit: async function(){
        //console.log('commit')
    }
}

Object.assign(db, events.prototype)

module.exports = db

