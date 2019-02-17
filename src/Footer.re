type showingState =
    | All
    | Active
    | Completed;

let component = ReasonReact.statelessComponent("Footer");

let push = (path, event) => {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReact.Router.push("#" ++ path);
}

let make = (~nowShowing, ~remaining, _children) => {
    ...component,
    render: _self => {
        let (all, active, completed) =
            switch(nowShowing) {
                | All => ("selected", "", "")
                | Active => ("", "selected", "")
                | Completed => ("", "", "selected")
            };

        <footer className="footer">
            <span className="todo-count">
                <strong>(ReasonReact.string(string_of_int(remaining)))</strong>
                (ReasonReact.string(" items left"))
            </span>

            <ul className="filters">
                <li>
                    <a onClick=(push("")) className=all>
                        (ReasonReact.string("All"))
                    </a>
                </li>
                <li>
                    <a onClick=(push("active")) className=active>
                        (ReasonReact.string("Active"))
                    </a>
                </li>
                <li>
                    <a onClick=(push("completed")) className=completed> 
                        (ReasonReact.string("Completed"))
                    </a>
                </li>
            </ul>
        </footer>
    }
}
