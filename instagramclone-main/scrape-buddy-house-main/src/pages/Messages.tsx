import { useState } from 'react';
import { Navigation } from '@/components/Navigation';
import { BottomNavigation } from '@/components/BottomNavigation';
import { Button } from '@/components/ui/button';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Dialog, DialogContent, DialogHeader, DialogTitle, DialogTrigger } from '@/components/ui/dialog';
import { Textarea } from '@/components/ui/textarea';
import { useToast } from '@/hooks/use-toast';

interface Note {
  id: string;
  username: string;
  avatar: string;
  text: string;
  timestamp: number;
}

interface Chat {
  id: string;
  username: string;
  avatar: string;
  lastMessage: string;
  time: string;
  unread: boolean;
}

const initialNotes: Note[] = [
  { id: '1', username: 'john_photo', avatar: '/placeholder.svg', text: 'Feeling creative today! 📸', timestamp: Date.now() },
  { id: '2', username: 'sarah_food', avatar: '/placeholder.svg', text: 'Coffee time ☕', timestamp: Date.now() },
  { id: '3', username: 'mike_fitness', avatar: '/placeholder.svg', text: 'Gym done! 💪', timestamp: Date.now() },
];

const chats: Chat[] = [
  {
    id: '1',
    username: 'john_photo',
    avatar: '/placeholder.svg',
    lastMessage: 'Hey! Love your recent post 📸',
    time: '1h ago',
    unread: true,
  },
  {
    id: '2',
    username: 'sarah_food',
    avatar: '/placeholder.svg',
    lastMessage: 'Thanks for the follow!',
    time: '3h ago',
    unread: true,
  },
  {
    id: '3',
    username: 'mike_fitness',
    avatar: '/placeholder.svg',
    lastMessage: 'Check out my new workout routine!',
    time: '1d ago',
    unread: false,
  },
  {
    id: '4',
    username: 'david_tech',
    avatar: '/placeholder.svg',
    lastMessage: 'Great content, keep it up! 🚀',
    time: '2d ago',
    unread: false,
  },
];

export default function Messages() {
  const [notes, setNotes] = useState<Note[]>(() => {
    const saved = localStorage.getItem('instagram_notes');
    return saved ? JSON.parse(saved) : initialNotes;
  });
  const [newNote, setNewNote] = useState('');
  const [isNoteDialogOpen, setIsNoteDialogOpen] = useState(false);
  const { toast } = useToast();

  const handleAddNote = () => {
    if (newNote.trim() && newNote.length <= 60) {
      const note: Note = {
        id: Date.now().toString(),
        username: 'emma_creative',
        avatar: '/placeholder.svg',
        text: newNote,
        timestamp: Date.now(),
      };
      const updatedNotes = [note, ...notes];
      setNotes(updatedNotes);
      localStorage.setItem('instagram_notes', JSON.stringify(updatedNotes));
      setNewNote('');
      setIsNoteDialogOpen(false);
      toast({ title: 'Note shared!' });
    }
  };

  return (
    <div className="min-h-screen bg-background pb-16">
      <Navigation />
      
      <main className="pt-14 max-w-2xl mx-auto">
        {/* Notes Section */}
        <div className="border-b border-border py-4">
          <div className="px-4 mb-3 flex items-center justify-between">
            <span className="text-sm font-semibold">Notes</span>
          </div>
          <ScrollArea className="w-full">
            <div className="flex gap-8 px-4 pb-2">
              {/* Your Note */}
              <Dialog open={isNoteDialogOpen} onOpenChange={setIsNoteDialogOpen}>
                <DialogTrigger asChild>
                  <button className="flex flex-col items-center gap-2 flex-shrink-0">
                    <div className="relative">
                      <Avatar className="h-14 w-14 ring-2 ring-primary">
                        <AvatarImage src="/placeholder.svg" />
                        <AvatarFallback>YN</AvatarFallback>
                      </Avatar>
                      <div className="absolute -bottom-1 left-1/2 -translate-x-1/2 bg-primary text-primary-foreground rounded-full px-2 py-0.5 text-xs whitespace-nowrap">
                        Your note
                      </div>
                    </div>
                  </button>
                </DialogTrigger>
                <DialogContent>
                  <DialogHeader>
                    <DialogTitle>Share a note</DialogTitle>
                  </DialogHeader>
                  <div className="space-y-4">
                    <Textarea
                      placeholder="Share a thought..."
                      value={newNote}
                      onChange={(e) => setNewNote(e.target.value)}
                      maxLength={60}
                      className="min-h-24"
                    />
                    <div className="flex items-center justify-between">
                      <span className="text-sm text-muted-foreground">
                        {newNote.length}/60
                      </span>
                      <Button onClick={handleAddNote} disabled={!newNote.trim()}>
                        Share
                      </Button>
                    </div>
                  </div>
                </DialogContent>
              </Dialog>

              {/* Other Users' Notes */}
              {notes.map((note) => (
                <button key={note.id} className="flex flex-col items-center gap-2 flex-shrink-0 group relative min-w-[70px]">
                  <div className="relative flex flex-col items-center">
                    <Avatar className="h-14 w-14 ring-2 ring-accent">
                      <AvatarImage src={note.avatar} />
                      <AvatarFallback>{note.username[0].toUpperCase()}</AvatarFallback>
                    </Avatar>
                    {/* Visible note bubble - truncated */}
                    <div className="absolute -bottom-4 left-1/2 -translate-x-1/2 bg-accent text-accent-foreground rounded-full px-3 py-1 text-xs max-w-[90px] text-center leading-tight transition-all group-hover:opacity-0 whitespace-nowrap">
                      {note.text.length > 15 ? note.text.substring(0, 15) + '...' : note.text}
                    </div>
                    {/* Full note on hover - can expand downwards */}
                    <div className="absolute -bottom-4 left-1/2 -translate-x-1/2 bg-accent text-accent-foreground rounded-lg px-3 py-2 text-xs max-w-[150px] text-center leading-tight opacity-0 group-hover:opacity-100 transition-all z-20">
                      {note.text}
                    </div>
                  </div>
                  <span className="text-xs text-muted-foreground mt-8">{note.username}</span>
                </button>
              ))}
            </div>
          </ScrollArea>
        </div>

        {/* Messages Section */}
        <div className="px-4 py-2">
          <div className="flex items-center justify-between mb-2">
            <h2 className="text-sm font-semibold">Messages</h2>
            <Button variant="ghost" size="sm" className="text-primary">
              Requests
            </Button>
          </div>
        </div>

        {/* Chat List */}
        <div>
          {chats.map((chat) => (
            <button
              key={chat.id}
              className="w-full flex items-center gap-3 p-4 hover:bg-muted/50 transition-colors"
            >
              <Avatar className="h-14 w-14 flex-shrink-0">
                <AvatarImage src={chat.avatar} />
                <AvatarFallback>{chat.username[0].toUpperCase()}</AvatarFallback>
              </Avatar>
              <div className="flex-1 min-w-0 text-left">
                <p className={`text-sm truncate ${chat.unread ? 'font-semibold' : ''}`}>
                  {chat.username}
                </p>
                <p className={`text-sm truncate ${chat.unread ? 'text-foreground font-medium' : 'text-muted-foreground'}`}>
                  {chat.lastMessage}
                </p>
              </div>
              <div className="flex flex-col items-end gap-1">
                <span className="text-xs text-muted-foreground">{chat.time}</span>
                {chat.unread && <div className="h-2 w-2 bg-primary rounded-full" />}
              </div>
            </button>
          ))}
        </div>
      </main>

      <BottomNavigation />
    </div>
  );
}
