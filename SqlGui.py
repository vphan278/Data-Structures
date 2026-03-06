import tkinter as tk
from tkinter import ttk
import sqlite3
from tkinter import font as tkfont
import pandas as pd

# -----------------------------
# Planet class
# -----------------------------
class Planet:
    def __init__(self, name, distance_au, orbit_years, image_path):
        self.name = name
        self.distance_au = distance_au
        self.orbit_years = orbit_years
        self.image_path = image_path


planet_instances = [
    Planet("Mercury", 0.39, 0.24, "images/mercury.jpg"),
    Planet("Venus", 0.72, 0.62, "images/venus.jpg"),
    Planet("Earth", 1.0, 1.0, "images/earth.jpg"),
    Planet("Mars", 1.52, 1.88, "images/mars.jpg"),
    Planet("Jupiter", 5.2, 11.86, "images/jupiter.jpg"),
    Planet("Saturn", 9.58, 29.46, "images/saturn.jpg"),
    Planet("Uranus", 19.22, 84.01, "images/uranus.jpg"),
    Planet("Neptune", 30.05, 164.8, "images/neptune.jpg"),
]

# Convert list of Planet objects → DataFrame
df_planets = pd.DataFrame([vars(p) for p in planet_instances])

# -----------------------------
# SQLite helpers
# -----------------------------
def create_connection(db_file: str) -> sqlite3.Connection:
    return sqlite3.connect(db_file)

def drop_table(conn: sqlite3.Connection, table_name: str) -> None:
    conn.execute(f"DROP TABLE IF EXISTS {table_name}")
    conn.commit()

# -----------------------------
# Tkinter Dashboard
# -----------------------------
class SQLiteDashboard:
    def __init__(self, root: tk.Tk):
        self.root = root
        self.root.title("SQLite DataFrame + Query Demo")

        # Global font
        default_font = tkfont.nametofont("TkDefaultFont")
        default_font.configure(family="Consolas", size=18, weight="bold")

        self.conn = create_connection("planets.db")
        self.table_name = "planets"

        # -----------------------------
        # Treeview Grid
        # -----------------------------
        style = ttk.Style()
        style.configure("Treeview", rowheight=32)

        self.table = ttk.Treeview(root, show="headings", height=12)
        self.table.grid(row=0, column=1, rowspan=20, padx=20, pady=10, sticky="nsew")

        scroll_y = ttk.Scrollbar(root, orient="vertical", command=self.table.yview)
        scroll_y.grid(row=0, column=2, rowspan=20, sticky="ns")

        scroll_x = ttk.Scrollbar(root, orient="horizontal", command=self.table.xview)
        scroll_x.grid(row=21, column=1, sticky="ew")

        self.table.configure(yscrollcommand=scroll_y.set, xscrollcommand=scroll_x.set)

        root.grid_columnconfigure(1, weight=1)
        root.grid_rowconfigure(0, weight=1)

        # -----------------------------
        # Buttons
        # -----------------------------
        button_specs = [
            ("Create Table (Schema Only)", self.gui_create_table),
            ("Insert Batch", self.gui_insert_batch),
            ("Insert Pluto", self.gui_insert_pluto),
            ("Search Earth", self.gui_search_earth),
            ("Update Earth", self.gui_update_earth),
            ("Delete Pluto", self.gui_delete_pluto),
            ("Drop Table", self.gui_drop_table),
        ]

        for i, (label, cmd) in enumerate(button_specs):
            ttk.Button(root, text=label, command=cmd).grid(row=i, column=0, sticky="ew", padx=10, pady=2)

    # -----------------------------
    # Dynamic column setup
    # -----------------------------
    def setup_columns(self, df: pd.DataFrame) -> None:
        cols = list(df.columns)
        self.table["columns"] = cols

        for col in cols:
            width = 180
            if col == "image_path":
                width = 500  # prevent truncation
            self.table.heading(col, text=col)
            self.table.column(col, width=width, anchor="center")

    # -----------------------------
    # Populate Treeview
    # -----------------------------
    def show_full_table(self, df: pd.DataFrame) -> None:
        for row in self.table.get_children():
            self.table.delete(row)

        for _, row in df.iterrows():
            self.table.insert("", "end", values=list(row))

    # -----------------------------
    # GUI Actions
    # -----------------------------
    def gui_create_table(self) -> None:
        df_planets.head(0).to_sql(self.table_name, self.conn, if_exists="replace", index=False)
        empty_df = df_planets.head(0)
        self.setup_columns(empty_df)
        self.show_full_table(empty_df)
        print("Created empty table from DataFrame schema.")

    def gui_insert_batch(self) -> None:
        df_planets.to_sql(self.table_name, self.conn, if_exists="append", index=False)
        df = pd.read_sql_query(f"SELECT * FROM {self.table_name}", self.conn)
        self.setup_columns(df)
        self.show_full_table(df)
        print("Inserted batch from DataFrame.")

    def gui_insert_pluto(self) -> None:
        df_pluto = pd.DataFrame([{
            "name": "Pluto",
            "distance_au": 39.48,
            "orbit_years": 248.0,
            "image_path": "images/pluto.jpg"
        }])
        df_pluto.to_sql(self.table_name, self.conn, if_exists="append", index=False)
        df = pd.read_sql_query(f"SELECT * FROM {self.table_name}", self.conn)
        self.setup_columns(df)
        self.show_full_table(df)
        print("Inserted Pluto.")

    def gui_search_earth(self) -> None:
        df = pd.read_sql_query(
            f"SELECT * FROM {self.table_name} WHERE name='Earth'",
            self.conn
        )

        if df.empty:
            print("Earth not found.")
            empty_df = df_planets.head(0)
            self.setup_columns(empty_df)
            self.show_full_table(empty_df)
            return

        # Highlight search result
        self.setup_columns(df)
        for row in self.table.get_children():
            self.table.delete(row)

        self.table.tag_configure("highlight", background="yellow")
        for _, row in df.iterrows():
            self.table.insert("", "end", values=list(row), tags=("highlight",))

        print("Displayed search result for Earth (highlighted).")

    def gui_update_earth(self) -> None:
        # safer: parameterized SQL
        self.conn.execute(
            f"UPDATE {self.table_name} SET distance_au=?, orbit_years=? WHERE name=?",
            (1.01, 1.01, "Earth")
        )
        self.conn.commit()

        df = pd.read_sql_query(f"SELECT * FROM {self.table_name}", self.conn)
        self.setup_columns(df)
        self.show_full_table(df)
        print("Updated Earth.")

    def gui_delete_pluto(self) -> None:
        self.conn.execute(f"DELETE FROM {self.table_name} WHERE name=?", ("Pluto",))
        self.conn.commit()

        df = pd.read_sql_query(f"SELECT * FROM {self.table_name}", self.conn)
        self.setup_columns(df)
        self.show_full_table(df)
        print("Deleted Pluto.")

    def gui_drop_table(self) -> None:
        drop_table(self.conn, self.table_name)
        self.table["columns"] = ()
        for row in self.table.get_children():
            self.table.delete(row)
        print("Dropped table and cleared grid.")

# -----------------------------
# Run the dashboard
# -----------------------------
if __name__ == "__main__":
    root = tk.Tk()
    SQLiteDashboard(root)
    root.mainloop()