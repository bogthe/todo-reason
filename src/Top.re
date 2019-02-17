open Belt;

type action =
    | NewTodoEnterKeyDown 
    | NewTodoOtherKeyDown
    | ChangeTodo(string);

type state = {
    newTodo: string,
    todos: list(TodoItem.todo)
}

let component = ReasonReact.reducerComponent("Top");

let make = _children => {
    ...component,
    initialState: () => { newTodo: "", todos: [] },
    reducer: (action, state) =>
    switch (action) {
        | ChangeTodo(todoText) => ReasonReact.Update({ ...state, newTodo: todoText })
        | NewTodoOtherKeyDown => ReasonReact.NoUpdate
        | NewTodoEnterKeyDown => {
            let todos =
                state.todos
                @
                [{
                    id: string_of_float(Js.Date.now()),
                    text: state.newTodo,
                    completed: false
                }]
            ;

            ReasonReact.Update({ todos, newTodo: "" });
        }
    },

    render: ({send, state}) => {
        let todosLength = List.length(state.todos);
        let todos = 
            state.todos
            |> List.map(_, (todo) => 
                <TodoItem key=todo.id todo />
            );

        let main = 
            todosLength === 0 ?
                ReasonReact.null :
                <section className="main">
                    <ul className="todo-list">
                        (ReasonReact.array(List.toArray(todos)))
                    </ul>
                </section>;

        <div>
            <header className="header">
            <h1> {ReasonReact.string("todos")} </h1>
            <input
                className="new-todo"
                placeholder="What needs to be done?"
                value={state.newTodo}
                onKeyDown={event =>
                    if (ReactEvent.Keyboard.keyCode(event) === 13) {
                        ReactEvent.Keyboard.preventDefault(event);
                        send(NewTodoEnterKeyDown);
                    } else {
                        send(NewTodoOtherKeyDown);
                    }
                }
                onChange={event =>
                    send(ChangeTodo(ReactEvent.Form.target(event)##value))
                }
                autoFocus=true
            />
            </header>
            main
        </div>;
    },
};