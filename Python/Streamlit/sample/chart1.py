import streamlit as st
import pandas as pd
import plotly.express as px

st.title("Mini MES Dashboard")

production = pd.DataFrame(
    {
        "Day":[1,2,3,4,5],
        "Production":[120,140,150,145,165]
    }
)

defect = pd.DataFrame(
    {
        "Day":[1,2,3,4,5],
        "Defect":[2.3,2.0,1.8,1.7,1.5]
    }
)

department = pd.DataFrame(
    {
        "Department":[
            "A라인",
            "B라인",
            "C라인"
        ],

        "Output":[
            150,
            180,
            130
        ]
    }
)

temperature = pd.DataFrame(
    {
        "Temperature":[20,25,30,35,40],

        "Defect":[1,2,3,5,8]
    }
)

st.subheader("생산량")
st.line_chart(production, x="Day", y="Production")

st.subheader("불량률")
st.area_chart(defect, x="Day", y="Defect")

st.subheader("라인별 생산량")
#st.bar_chart(department, x="Department", y="Output")
fig_bar = px.bar(
    department, 
    x="Department", 
    y="Output",
    color='Department',
    color_discrete_sequence=['red', 'blue', 'green']
)
st.plotly_chart(fig_bar)

st.subheader("온도와 불량률")
st.scatter_chart(
    temperature,
    x="Temperature",
    y="Defect"
)
