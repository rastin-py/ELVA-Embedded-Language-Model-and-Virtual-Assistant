import openai

client = openai.OpenAI(
    base_url="http://localhost:11434/v1",
    api_key="nokeyneeded",
)

conversation_history = [
    {"role": "system",
     "content": "Your name is Elva which stands for Embedded Language Model powered Virtual Assistant. You are a "
                "highly advanced AI. Keep conversations short, friendly and non-formal."}
]


def run_ai(user_input):
    # Append the user's message to the conversation history
    conversation_history.append({"role": "user", "content": user_input})

    # Call the API with the entire conversation history
    response = client.chat.completions.create(
        model="llama3.2:1b",
        temperature=1,
        n=1,
        messages=conversation_history
    )

    # Get the assistant's reply from the response
    assistant_reply = response.choices[0].message.content

    # Append the assistant's reply to the conversation history
    conversation_history.append({"role": "assistant", "content": assistant_reply})

    return assistant_reply
