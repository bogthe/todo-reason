open Belt;

type action =
    | NewTodoEnterKeyDown 
    | NewTodoOtherKeyDown
    | Toggle(TodoItem.todo)
    | Delete(TodoItem.todo)
    | Navigate(Footer.showingState)
    | ChangeTodo(string);

type state = {
    newTodo: string,
    todos: list(TodoItem.todo),
    nowShowing: Footer.showingState
}

let urlShownOnPage = path =>
    switch(path) {
        | "active" => Footer.Active
        | "completed" => Footer.Completed
        | _ => Footer.All
    };

// set initial state to current url
// watch for url changes on did mount
// pass current url state to any components needed

let component = ReasonReact.reducerComponent("Top");

let make = _children => {
    ...component,
    initialState: () => {
        newTodo: "",
        todos: [],
        nowShowing: urlShownOnPage(ReasonReact.Router.dangerouslyGetInitialUrl().hash)
    },
    didMount: (self) => {
        let watchId =
            ReasonReact.Router.watchUrl(url =>
                self.send(Navigate(urlShownOnPage(url.hash)))
            );

        self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
    },

    reducer: (action, state) =>
        switch (action) {
            | ChangeTodo(todoText) => ReasonReact.Update({ ...state, newTodo: todoText })
            | Navigate(currentUrl) => ReasonReact.Update({ ...state, nowShowing: currentUrl })
            | NewTodoOtherKeyDown => ReasonReact.NoUpdate
            | NewTodoEnterKeyDown =>
                switch(String.trim(state.newTodo)) {
                    | "" => ReasonReact.NoUpdate
                    | nonEmptyValue => {
                        let todos =
                            state.todos
                            @
                            [{
                                id: string_of_float(Js.Date.now()),
                                text: nonEmptyValue,
                                completed: false
                            }]

                        ;
                        ReasonReact.Update({ ...state, todos, newTodo: "" });
                    }
                }
            | Toggle(todoItem) => {
                let todos = List.map(state.todos, todo =>
                    todo == todoItem ? {...todo, TodoItem.completed: !todo.completed} : todo
                );

                ReasonReact.Update({ ...state, todos });
            }
            | Delete(todoItem) => {
                let todos = List.keep(state.todos, todo => todo != todoItem);
                ReasonReact.Update({ ...state, todos });
            }
        },

    render: ({send, state}) => {
        let todosLength = List.length(state.todos);
        let remaining =
            List.keep(state.todos, todo => !todo.completed)
            |> List.length;

        let todos = 
            state.todos
            |> List.keep(_, todo =>
                switch(state.nowShowing) {
                    | All => true
                    | Active => !todo.completed
                    | Completed => todo.completed
                }
            )
            |> List.map(_, todo => 
                <TodoItem
                    key=todo.id
                    todo
                    onToggle=(_event => send(Toggle(todo)))
                    onDestroy=(_event => send(Delete(todo)))
                />
            );

        let main = 
            todosLength === 0 ?
                ReasonReact.null :
                <section className="main">
                    <ul className="todo-list">
                        (ReasonReact.array(List.toArray(todos)))
                    </ul>
                </section>;

        let footer =
            todosLength === 0 ?
                ReasonReact.null :
                <Footer nowShowing=state.nowShowing remaining />;

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
            footer
        </div>;
    },
};