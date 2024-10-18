
let db = {
    data: [],
    currentFreeId: 0,

    select: function(){
        return this.data;
    },
    insert: function(item){
        let completedItem = {
            id: this.currentFreeId,
            name: item.name,
            bday: item.bday
        }
        this.currentFreeId++;
        this.data.push(completedItem);
        return completedItem;
    },
    update: function(newString){
        let index = this.data.findIndex(item => item.id === newString.id);
        if (index !== -1) {
            this.data[index].name = newString.name;
            this.data[index].bday = newString.bday;
        } else {
            console.log('Item not found');
        }
    },
    delete: function(id){
        let index = this.data.findIndex(item => item.id.toString() === id);
        let retItem;
        if (index !== -1) {
            retItem = {
                id: this.data[index].id,
                name: this.data[index].name,
                bday: this.data[index].bday,
            }
            this.data.splice(index, 1);
        } else {
            console.log('Item not found');
        }
        return retItem;
    }
}

module.exports = db;