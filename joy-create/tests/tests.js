import { ListNode, LinkedList } from './lsystem.js'

var list = new LinkedList();
list.add(new ListNode(0));
list.add(new ListNode(1));
list.add(new ListNode(2));

console.log(list.getAt(2));
console.log(list.size());