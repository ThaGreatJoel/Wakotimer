import { useState } from "react";
import { useRouter } from "next/router";

export default function Home() {
  const [email, setEmail] = useState("");
  const [apiKey, setApiKey] = useState("");
  const router = useRouter();

  async function handleSubmit(e) {
    e.preventDefault();
    const res = await fetch("/api/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ email, hackaTimeApiKey: apiKey })
    });

    if (res.ok) router.push(`/dashboard?apiKey=${apiKey}`);
  }

  return (
    <form onSubmit={handleSubmit}>
      <input placeholder="Email" value={email} onChange={e => setEmail(e.target.value)} />
      <input placeholder="HackaTime API Key" value={apiKey} onChange={e => setApiKey(e.target.value)} />
      <button type="submit">Login</button>
    </form>
  );
}
