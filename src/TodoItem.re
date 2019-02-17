type todo = {
    id: string,
    text: string,
    completed: bool
};

let component = ReasonReact.statelessComponent("TodoItem");
let make = (
    ~todo,
    ~onToggle,
    ~onDestroy,
    _children
    ) => {
    ...component,
    render: _self => {
        let className =
            [ todo.completed ? "completed" : ""]
            |> String.concat(" ");

        <li className>
            <div className="view"> 
                <input
                    className="toggle"
                    type_="checkbox"
                    checked=todo.completed
                    onChange=(_ => onToggle())
                />
                <label>
                    (ReasonReact.string(todo.text))
                </label>
                <button
                    className="destroy"
                    onClick=(_ => onDestroy())
                />
            </div>
        </li>
    }
};
