type todo = {
    id: string,
    text: string,
    completed: bool
};

let component = ReasonReact.statelessComponent("TodoItem");
let make = (~todo, _children) => {
    ...component,
    render: _self =>
        <li>
            <div className="view"> 
                <input
                    className="toggle"
                    type_="checkbox"
                    checked=todo.completed
                />
                <label>
                    (ReasonReact.string(todo.text))
                </label>
                <button className="destroy" />
            </div>
        </li>
};
